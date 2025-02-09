import torch
from torch import Tensor
import torch.nn as nn

class dice_multi(nn.Module):
    def __init__(self, n_classes=3):
        super(dice_multi, self).__init__()
        self.smooth = 1e-7
        self.n_classes = n_classes

    def forward(self, y_pred, y_true):
        # y_pred: (N, C, H, W) - logity
        # y_true: (N, H, W) - indeksy klas (np. 0, 1, 2)
        
        y_pred = torch.softmax(y_pred, dim=1)  # Softmax na logitach
        dice = 0.0
        
        # Konwersja masek na one-hot encoding
        y_true_onehot = torch.zeros_like(y_pred)
        y_true_onehot.scatter_(1, y_true.unsqueeze(1), 1)  # (N, C, H, W)
        
        for c in range(self.n_classes):
            pred_c = y_pred[:, c, ...].contiguous().view(-1)
            true_c = y_true_onehot[:, c, ...].contiguous().view(-1)
            intersection = (pred_c * true_c).sum()
            union = pred_c.sum() + true_c.sum()
            dice += (2. * intersection + self.smooth) / (union + self.smooth)
        
        dice /= self.n_classes
        return 1. - dice