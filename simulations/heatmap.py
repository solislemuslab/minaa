#!/usr/bin/env python
# coding: utf-8

# In[1]:

import numpy as np
import pandas as pd
import seaborn as sns
import matplotlib.pyplot as plt
import os
import matplotlib.pyplot as plt
import warnings
warnings.filterwarnings('ignore')

# In[2]:

path = 'alignments/'

# In[3]:

result10_05 = np.zeros((10, 10))
for file in os.listdir(path):
    if (file[0:9] == 'G-10-0.05'):
        path_r = path+file+'/'
        for file_r in os.listdir(path_r):
            if (file_r == 'alignment_matrix.csv'):
                net = pd.read_csv(path_r+file_r, index_col=0)
                result10_05 = result10_05+net
result10_05 = result10_05/30

result10_1 = np.zeros((10, 10))
for file in os.listdir(path):
    if (file[0:8] == 'G-10-0.1'):
        path_r = path+file+'/'
        for file_r in os.listdir(path_r):
            if (file_r == 'alignment_matrix.csv'):
                net = pd.read_csv(path_r+file_r, index_col=0)
                result10_1 = result10_1+net
result10_1 = result10_1/30

result10_9 = np.zeros((10, 10))
for file in os.listdir(path):
    if (file[0:8] == 'G-10-0.9'):
        path_r = path+file+'/'
        for file_r in os.listdir(path_r):
            if (file_r == 'alignment_matrix.csv'):
                net = pd.read_csv(path_r+file_r, index_col=0)
                result10_9 = result10_9+net
result10_9 = result10_9/30

# In[4]:

result30_05 = np.zeros((30, 30))
for file in os.listdir(path):
    if (file[0:9] == 'G-30-0.05'):
        path_r = path+file+'/'
        for file_r in os.listdir(path_r):
            if (file_r == 'alignment_matrix.csv'):
                net = pd.read_csv(path_r+file_r, index_col=0)
                result30_05 = result30_05+net
result30_05 = result30_05/30

result30_1 = np.zeros((30, 30))
for file in os.listdir(path):
    if (file[0:8] == 'G-30-0.1'):
        path_r = path+file+'/'
        for file_r in os.listdir(path_r):
            if (file_r == 'alignment_matrix.csv'):
                net = pd.read_csv(path_r+file_r, index_col=0)
                result30_1 = result30_1+net
result30_1 = result30_1/30

result30_9 = np.zeros((30, 30))
for file in os.listdir(path):
    if (file[0:8] == 'G-30-0.9'):
        path_r = path+file+'/'
        for file_r in os.listdir(path_r):
            if (file_r == 'alignment_matrix.csv'):
                net = pd.read_csv(path_r+file_r, index_col=0)
                result30_9 = result30_9+net
result30_9 = result30_9/30

# In[5]:

result50_05 = np.zeros((50, 50))
for file in os.listdir(path):
    if (file[0:9] == 'G-50-0.05'):
        path_r = path+file+'/'
        for file_r in os.listdir(path_r):
            if (file_r == 'alignment_matrix.csv'):
                net = pd.read_csv(path_r+file_r, index_col=0)
                result50_05 = result50_05+net
result50_05 = result50_05/30

result50_1 = np.zeros((50, 50))
for file in os.listdir(path):
    if (file[0:8] == 'G-50-0.1'):
        path_r = path+file+'/'
        for file_r in os.listdir(path_r):
            if (file_r == 'alignment_matrix.csv'):
                net = pd.read_csv(path_r+file_r, index_col=0)
                result50_1 = result50_1+net
result50_1 = result50_1/30

result50_9 = np.zeros((50, 50))
for file in os.listdir(path):
    if (file[0:8] == 'G-50-0.9'):
        path_r = path+file+'/'
        for file_r in os.listdir(path_r):
            if (file_r == 'alignment_matrix.csv'):
                net = pd.read_csv(path_r+file_r, index_col=0)
                result50_9 = result50_9+net
result50_9 = result50_9/30

# In[6]:

result100_05 = np.zeros((100, 100))
for file in os.listdir(path):
    if (file[0:10] == 'G-100-0.05'):
        path_r = path+file+'/'
        for file_r in os.listdir(path_r):
            if (file_r == 'alignment_matrix.csv'):
                net = pd.read_csv(path_r+file_r, index_col=0)
                result100_05 = result100_05+net
result100_05 = result100_05/30

result100_1 = np.zeros((100, 100))
for file in os.listdir(path):
    if (file[0:9] == 'G-100-0.1'):
        path_r = path+file+'/'
        for file_r in os.listdir(path_r):
            if (file_r == 'alignment_matrix.csv'):
                net = pd.read_csv(path_r+file_r, index_col=0)
                result100_1 = result100_1+net
result100_1 = result100_1/30

result100_9 = np.zeros((100, 100))
for file in os.listdir(path):
    if (file[0:9] == 'G-100-0.9'):
        path_r = path+file+'/'
        for file_r in os.listdir(path_r):
            if (file_r == 'alignment_matrix.csv'):
                net = pd.read_csv(path_r+file_r, index_col=0)
                result100_9 = result100_9+net
result100_9 = result100_9/30

# In[7]:

result250_05 = np.zeros((250, 250))
for file in os.listdir(path):
    if (file[0:10] == 'G-250-0.05'):
        path_r = path+file+'/'
        for file_r in os.listdir(path_r):
            if (file_r == 'alignment_matrix.csv'):
                net = pd.read_csv(path_r+file_r, index_col=0)
                result250_05 = result250_05+net
result250_05 = result250_05/30

result250_1 = np.zeros((250, 250))
for file in os.listdir(path):
    if (file[0:9] == 'G-250-0.1'):
        path_r = path+file+'/'
        for file_r in os.listdir(path_r):
            if (file_r == 'alignment_matrix.csv'):
                net = pd.read_csv(path_r+file_r, index_col=0)
                result250_1 = result250_1+net
result250_1 = result250_1/30

result250_9 = np.zeros((250, 250))
for file in os.listdir(path):
    if (file[0:9] == 'G-250-0.9'):
        path_r = path+file+'/'
        for file_r in os.listdir(path_r):
            if (file_r == 'alignment_matrix.csv'):
                net = pd.read_csv(path_r+file_r, index_col=0)
                result250_9 = result250_9+net
result250_9 = result250_9/30

# In[8]:

result500_05 = np.zeros((500, 500))
for file in os.listdir(path):
    if (file[0:10] == 'G-500-0.05'):
        path_r = path+file+'/'
        for file_r in os.listdir(path_r):
            if (file_r == 'alignment_matrix.csv'):
                net = pd.read_csv(path_r+file_r, index_col=0)
                result500_05 = result500_05+net
result500_05 = result500_05/30

result500_1 = np.zeros((500, 500))
for file in os.listdir(path):
    if (file[0:9] == 'G-500-0.1'):
        path_r = path+file+'/'
        for file_r in os.listdir(path_r):
            if (file_r == 'alignment_matrix.csv'):
                net = pd.read_csv(path_r+file_r, index_col=0)
                result500_1 = result500_1+net
result500_1 = result500_1/30

result500_9 = np.zeros((500, 500))
for file in os.listdir(path):
    if (file[0:9] == 'G-500-0.9'):
        path_r = path+file+'/'
        for file_r in os.listdir(path_r):
            if (file_r == 'alignment_matrix.csv'):
                net = pd.read_csv(path_r+file_r, index_col=0)
                result500_9 = result500_9+net
result500_9 = result500_9/30

# In[9]:

data_dic = {
    'result10_05': result10_05,
    'result30_05': result30_05,
    'result50_05': result50_05,
    'result100_05': result100_05,
    'result250_05': result250_05,
    'result500_05': result500_05,
    'result10_1': result10_1,
    'result30_1': result30_1,
    'result50_1': result50_1,
    'result100_1': result100_1,
    'result250_1': result250_1,
    'result500_1': result500_1,
    'result10_9': result10_9,
    'result30_9': result30_9,
    'result50_9': result50_9,
    'result100_9': result100_9,
    'result250_9': result250_9,
    'result500_9': result500_9
}

# In[10]:

data_keys = [key for key in data_dic.keys()]
data_labels = []

# In[17]:

fig, axn = plt.subplots(3, 6, figsize=(25, 13), sharex=False, sharey=False)
cbar_ax = fig.add_axes([.91, .3, .01, .4])
fig.subplots_adjust(hspace=0.05, wspace=0.08)

for i, ax in enumerate(axn.flat):
    ax = plt.subplot(3, 6, i+1)
    sns.heatmap(data_dic[data_keys[i]],
                ax=ax,
                cbar=i == 0,
                vmin=0,
                vmax=1,
                cbar_ax=None if i else cbar_ax,
                square=True,
                cmap='ocean_r')
    cbar_ax.tick_params(labelsize=20)
    plt.xlabel('')
    ax.axes.yaxis.set_ticklabels([])
    ax.axes.xaxis.set_ticklabels([])

    if i == 0:
        plt.ylabel('0.05', fontsize=25, rotation=90)
    if i == 6:
        plt.ylabel('0.1', fontsize=25)
    if i == 12:
        plt.ylabel('0.9', fontsize=25)
        plt.xlabel('10 nodes', fontsize=25)
    if i == 13:
        plt.xlabel('30 nodes', fontsize=25)
    if i == 14:
        plt.xlabel('50 nodes', fontsize=25)
    if i == 15:
        plt.xlabel('100 nodes', fontsize=25)
    if i == 16:
        plt.xlabel('250 nodes', fontsize=25)
    if i == 17:
        plt.xlabel('500 nodes', fontsize=25)

plt.savefig('heatmap.png', dpi=200, pad_inches=0, facecolor='white')
