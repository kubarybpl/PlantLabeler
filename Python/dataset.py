from torch.utils.data import Dataset
from torchvision import datasets
from torchvision.transforms import ToTensor
import torch
import os
import numpy as np
from skimage.io import imread
from PIL import Image

class plant_dataset(Dataset):
    def __init__(self, train = True, data_set= set(), validation_dir = ''):
        self.validation_dir = validation_dir
        self.data_set = data_set
        self.train = train
        self.masks = []
        self.images = []   

        if self.train == True:
            for image_path in data_set:
                self.images.append(imread(image_path))

                directory = os.path.dirname(image_path)
                file_name = os.path.basename(image_path)
                mask_name = "mask_" + file_name
                mask_path = os.path.join(directory, mask_name)
                mask = Image.open(mask_path)
                mask = mask.convert('P')
                mask = np.array(mask, dtype=np.uint8)
                self.masks.append(mask)

        else:            
            for (dir_path, dirnames, file_names) in os.walk(self.validation_dir):
                for file_name in file_names:
                    if file_name.startswith('mask_') :
                        mask_path = os.path.join(dir_path, file_name)
                        image_path = os.path.join(dir_path, file_name[5:])
                        if os.path.exists(image_path) :
                            self.images.append(imread(image_path))
                            
                            mask = Image.open(mask_path)
                            mask = mask.convert('P')
                            mask = np.array(mask, dtype=np.uint8)
                            self.masks.append(mask)
            
    def __len__(self):
        return len(self.images)

    def __getitem__(self, idx):
        image = self.images[idx]
        mask = self.masks[idx]
        
        image = ToTensor()(image)
        mask = torch.from_numpy(mask).long()

        return image, mask