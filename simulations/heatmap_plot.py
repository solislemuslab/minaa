#!/usr/bin/env python
# coding: utf-8

# In[13]:
import numpy as np
import pandas as pd
import seaborn as sns
import matplotlib.pyplot as plt
import os
import openpyxl
import matplotlib.pyplot as plt
import warnings
warnings.filterwarnings('ignore')

# In[15]:
result10_05=np.zeros((10, 10))
for file in os.listdir(path):
    if (file[0:14] == 'Graph1-10-0.05'): 
        path_r=path+file+'/'
        #print(path_r)
        for file_r in os.listdir(path_r):
            if (file_r == 'alignment_matrix.csv'):
                net=pd.read_csv(path_r+file_r, index_col=0)
                result10_05=result10_05+net
result10_05=result10_05/30
########################################
result10_1=np.zeros((10, 10))
for file in os.listdir(path):
    if (file[0:13] == 'Graph1-10-0.1'): 
        path_r=path+file+'/'
        #print(path_r)
        for file_r in os.listdir(path_r):
            if (file_r == 'alignment_matrix.csv'):
                net=pd.read_csv(path_r+file_r, index_col=0)
                result10_1=result10_1+net
result10_1=result10_1/30
########################################
result10_9=np.zeros((10, 10))
for file in os.listdir(path):
    if (file[0:13] == 'Graph1-10-0.9'): 
        path_r=path+file+'/'
        #print(path_r)
        for file_r in os.listdir(path_r):
            if (file_r == 'alignment_matrix.csv'):
                net=pd.read_csv(path_r+file_r, index_col=0)
                result10_9=result10_9+net
result10_9=result10_9/30

# In[16]:
result30_05=np.zeros((30, 30))
for file in os.listdir(path):
    if (file[0:14] == 'Graph1-30-0.05'): 
        path_r=path+file+'/'
        #print(path_r)
        for file_r in os.listdir(path_r):
            if (file_r == 'alignment_matrix.csv'):
                net=pd.read_csv(path_r+file_r, index_col=0)
                result30_05=result30_05+net
result30_05=result30_05/30

result30_1=np.zeros((30, 30))
for file in os.listdir(path):
    if (file[0:13] == 'Graph1-30-0.1'): 
        path_r=path+file+'/'
        #print(path_r)
        for file_r in os.listdir(path_r):
            if (file_r == 'alignment_matrix.csv'):
                net=pd.read_csv(path_r+file_r, index_col=0)
                result30_1=result30_1+net
result30_1=result30_1/30

result30_9=np.zeros((30, 30))
for file in os.listdir(path):
    if (file[0:13] == 'Graph1-30-0.9'): 
        path_r=path+file+'/'
        #print(path_r)
        for file_r in os.listdir(path_r):
            if (file_r == 'alignment_matrix.csv'):
                net=pd.read_csv(path_r+file_r, index_col=0)
                result30_9=result30_9+net
result30_9=result30_9/30

# In[17]:
result50_05=np.zeros((50, 50))
for file in os.listdir(path):
    if (file[0:14] == 'Graph1-50-0.05'): 
        path_r=path+file+'/'
        #print(path_r)
        for file_r in os.listdir(path_r):
            if (file_r == 'alignment_matrix.csv'):
                net=pd.read_csv(path_r+file_r, index_col=0)
                result50_05=result50_05+net
result50_05=result50_05/30

result50_1=np.zeros((50, 50))
for file in os.listdir(path):
    if (file[0:13] == 'Graph1-50-0.1'): 
        path_r=path+file+'/'
        #print(path_r)
        for file_r in os.listdir(path_r):
            if (file_r == 'alignment_matrix.csv'):
                net=pd.read_csv(path_r+file_r, index_col=0)
                result50_1=result50_1+net
result50_1=result50_1/30

result50_9=np.zeros((50, 50))
for file in os.listdir(path):
    if (file[0:13] == 'Graph1-50-0.9'): 
        path_r=path+file+'/'
        #print(path_r)
        for file_r in os.listdir(path_r):
            if (file_r == 'alignment_matrix.csv'):
                net=pd.read_csv(path_r+file_r, index_col=0)
                result50_9=result50_9+net
result50_9=result50_9/30

# In[18]:
data_dic = {
    'result10_05':result10_05,
    'result30_05':result30_05,
    'result50_05':result50_05,
    'result10_1':result10_1,
    'result30_1':result30_1,
    'result50_1':result50_1,
    'result10_9':result10_9,
    'result30_9':result30_9,
    'result50_9':result50_9
}

# In[19]:
data_keys = [key for key in data_dic.keys()]
data_labels = []


# In[20]:
fig, axn = plt.subplots(3, 3, figsize=(15,15), sharex=False, sharey=False)
cbar_ax = fig.add_axes([.91, .3, .01, .4])
fig.subplots_adjust(hspace =0.01, wspace=0.02)

for i, ax in enumerate(axn.flat):
    ax = plt.subplot(3,3,i+1)
    sns.heatmap(data_dic[data_keys[i]],
                     ax=ax,
                         cbar = i==0, 
                         linewidths=.1, 
                         vmin=0, 
                         vmax=1,
                         cbar_ax=None if i else cbar_ax,
               cmap='Blues')
    plt.xlabel('')
    ax.axes.yaxis.set_ticklabels([])
    ax.axes.xaxis.set_ticklabels([])
    if i==0:
        plt.ylabel('0.05',fontsize=20,rotation=90) 
    if i==3:
        plt.ylabel('0.1',fontsize=20)
    if i==6:
        plt.ylabel('0.9',fontsize=20)
        plt.xlabel('10 nodes',fontsize=20)
    if i==7:
        plt.xlabel('30 nodes',fontsize=20) 
    if i==8:
        plt.xlabel('50 nodes',fontsize=20)
plt.savefig('heatmapGraph.png',dpi=600,pad_inches=0,facecolor='white')
