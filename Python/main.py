import sys
import time
print("Uruchamianie skrytpu")
sys.stdout.flush()
import os
from collections import OrderedDict
import csv

import torch
import torch.nn as nn
import torch.nn.functional as F
import numpy as np
from skimage.io import imread
from torch.utils.data import DataLoader
import torch.optim as optim


from unet import UNet
from dataset import plant_dataset
from dice_multi import dice_multi

import argparse

def get_args():
    parser = argparse.ArgumentParser()
    parser.add_argument("--data_dir", help="Path to data directory")
    parser.add_argument("--model_path", help="Path to models directory")
    parser.add_argument("--csv_path", help="Path to csv file containining table of used data")
    parser.add_argument("--validation_dir", help="Path to validation dataset")
    return parser.parse_args()

def get_file_set(data_dir):
    file_paths =[]
    for (dir_path, dirnames, file_names) in os.walk(data_dir):
              for file_name in file_names:
                if file_name.startswith('mask_') :
                    image_path = os.path.join(dir_path, file_name[5:])
                    if os.path.exists(image_path) :
                        file_paths.append(image_path)
    return set(file_paths)

def get_csv_set(csv_path):
    file_paths = set()
    with open(csv_path, mode = 'r',  newline='', encoding='utf-8') as file:
        csv_reader = csv.reader(file)
        for line in csv_reader:
            file_paths.add(line[0])

    return set(file_paths)

def update_csv(csv_path, new_set):
    print(f'{len(new_set)} new files')
    sys.stdout.flush()
    time.sleep(2)
    with open(csv_path, mode = 'a', newline='', encoding='utf-8') as file:
        writer = csv.writer(file)
        for file_name in new_set:
            writer.writerow([file_name])
        


def train_loop(dataloader, model, loss_fn, optimizer, batch_size = 8):
    size = len(dataloader.dataset)
    
    model.train()
    total_loss = 0.0
    total_samples = 0
    for batch, (X, y) in enumerate(dataloader):
        X = X.to(device)
        y = y.to(device)
        
        optimizer.zero_grad()  # Wyzerowanie gradientów
        pred = model(X)

        loss = alpha * loss_fn(pred, y)
        loss += beta * dice_multi(pred, y)
        
        # Backpropagacja i optymalizacja
        loss.backward()
        optimizer.step()

        total_loss += loss
        total_samples += len(X)
    avg_loss = total_loss/total_samples
    print(f'Average training loss: {avg_loss:>7f}')
    sys.stdout.flush()

    # return avg_loss


def validation_loop(dataloader, model, loss_fn):
    model.eval()
    size = len(dataloader.dataset)
    num_batches = len(dataloader)
    loss = 0.0
    total_samples = 0
    total_loss = 0.0
    with torch.no_grad():
        for X, y in dataloader:
            X, y = X.to(device), y.to(device)
            pred = model(X)
            
            # loss = loss_fn(pred, y)
            loss = alpha * loss_fn(pred, y)
            loss += beta * dice_multi(pred, y)
            
            total_loss += loss * X.size(0)
            total_samples += X.size(0)

    avg_loss = total_loss/total_samples
    print(f'Average valdiation loss: {avg_loss:>7f}')
    sys.stdout.flush()
    # return avg_loss

if __name__ == "__main__":
    args = get_args()

    # Check if new files exists
    file_set = get_file_set(args.data_dir)
    csv_set = get_csv_set(args.csv_path)
    new_set = file_set - csv_set

    if len(new_set) != 0:
        print(f'{len(new_set)} nowych masek')
        # Check if CUDA available
        device = torch.device("cuda" if torch.cuda.is_available() else "cpu")

        if device == 'cuda':
            print("CUDA jest w użyciu")
            sys.stdout.flush()
        else:
            print("CPU jest w użyciu")
            sys.stdout.flush()


        # Datasets
        dataset_train = plant_dataset(train = True, data_set=new_set, validation_dir=args.validation_dir)
        dataset_test = plant_dataset(train = False, data_set=new_set, validation_dir=args.validation_dir)
        
        # Loss functions
        dice_multi = dice_multi(3)
        loss_fn = nn.CrossEntropyLoss()
        alpha = 0.5 # cross entropy coef
        beta = 0.5 # dice loss coef

        unet = torch.jit.load(args.model_path + "unet_init.pt").to(device)
        mobile_net = torch.jit.load(args.model_path + "mobilenet_init.pt").to(device)

        # Training parameters
        batch_size = 8
        epochs = 20
        learning_rate = 0.001
        optimizer = optim.Adam(unet.parameters(), lr = learning_rate)

        # Data loaders
        loaders = {
        "train": DataLoader(dataset_train, batch_size=batch_size, shuffle=True, generator=torch.Generator(device='cpu')),
        "test": DataLoader(dataset_test, batch_size=batch_size, shuffle=False, generator=torch.Generator(device='cpu')),
        }

        # Fine tune
        # Doucz i stwórz model model_temp
        # Porównaj dokładność, jeżeli większa to 
        # model_temp. old = model_temp
        # unet = model_temp
        # jeżeli nie
        # model_temp. old = model_temp
                
        for t in range(epochs):
            print(f"Epoch {t+1} -------------------------------")
            print("UNet Model:")
            train_loop(loaders["train"], unet, loss_fn, optimizer = optimizer, batch_size = batch_size)
            validation_loop(loaders["test"], unet, loss_fn)
            print("MoblieNet Model:")
            train_loop(loaders["train"], mobile_net, loss_fn, optimizer = optimizer, batch_size = batch_size)
            validation_loop(loaders["test"], mobile_net, loss_fn)
            

        unet.save(args.model_path + "unet_init.pt")
        mobile_net.save(args.model_path + "mobilenet_init.pt")

        update_csv(args.csv_path, new_set)

        print("Zadanie zakończone pomyślnie!")
        sys.stdout.flush()
    else:
        print('Brak nowych masek')
        sys.stdout.flush()
