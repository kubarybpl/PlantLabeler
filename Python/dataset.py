from torch.utils.data import Dataset
from torchvision.transforms import ToTensor
import matplotlib.pyplot as plt
import os
import numpy as np
from skimage.io import imread
import sys
import time
class plantDataset(Dataset):
    def __init__(self, dataDir, train = True, transform=None, target_transform=None):
        self.DataDir = dataDir
        self.transform = transform
        self.target_transform = target_transform
        self.Train = train
        self.masks = []
        self.images = []   
        print(self.DataDir)
        sys.stdout.flush()
        time.sleep(1)

        for (dirpath, dirnames, filenames) in os.walk(self.DataDir):
              for filename in filenames:
                if filename.startswith('mask_') :
                    maskpath = os.path.join(dirpath, filename)
                    imagepath = os.path.join(dirpath, filename[5:])
                    if os.path.exists(imagepath) :
                        self.images.append(imread(imagepath))
                        self.masks.append(imread(maskpath))


            
    def __len__(self):
        return len(self.images)

    def __getitem__(self, idx):
        image = self.images[idx]
        mask = self.masks[idx]
        
        image = ToTensor()(image)
        mask = ToTensor()(mask)

        return image, mask