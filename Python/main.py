import sys
import time
import torch
import torch.nn as nn
import torch.nn.functional as F
from collections import OrderedDict
import numpy as np

from unet import UNet
from dataset import plantDataset

import argparse
def get_args():
    parser = argparse.ArgumentParser()
    parser.add_argument("--data_dir", help="Path to data directory")
    parser.add_argument("--model_dir", help="Path to models directory")
    parser.add_argument("--csv_file", help="Path to csv file containining table of used data")
    return parser.parse_args()

if __name__ == "__main__":
    
    print("parsing arguments")
    sys.stdout.flush()
    args = get_args()

    unet = UNet(in_channels=3, out_channels=3)
    dataSet = plantDataset(args.data_dir)


    print("po import torch")
    sys.stdout.flush()

    print("Rozpoczynam pracę...")
    sys.stdout.flush()

    for i in range(5):
        print(f"Postęp: {i+1}/5")
        sys.stdout.flush()
        time.sleep(0.5)

    print("Zadanie zakończone pomyślnie!")
    sys.stdout.flush()


