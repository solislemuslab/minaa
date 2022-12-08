#rm(list=ls())
library(SpiecEasi)
source('adjacency_matrix_from_data_frame.R')
#library(igraph)

## Simulated Networks ##

########Parameter
numberofNodeT=c(10,30,50)
DPT=c(0.05,0.1,0.9)
######################################
for (i in 1:length(numberofNodeT)){
  for (j in 1:length(DPT)){
    for (rep in 1:30){
        set.seed(rep)
        numberofNode=numberofNodeT[i]
        DP=DPT[j]
        e <- d<-numberofNode
        ##numberofNode=numberofEdge
        #Synthesize the Net
        graph1 <- SpiecEasi::make_graph('cluster', d, e)
        rownames(graph1)=colnames(graph1) =paste0("V",1:numberofNode)
        write.csv(graph1,paste0('network/Graph1-',numberofNode,'-',DP,'-',rep,'.csv'))
        e1=sum(graph1)
        k1=which(graph1>0)
        k10=which(graph1==0)
        graph2=graph1
        per=round(sum(graph2)*DP)
        se1= sample(k1,per)
        se10= sample(k10,per)
        graph2[se1]=0
        graph2[se10]=1
        
        graph2[lower.tri(graph2)] = t(graph2)[lower.tri(graph2)]
        diag(graph2) <- 0
        e2=sum(graph2)
        e12=sum(graph1==graph2)/(e*d)
        rownames(graph2)=colnames(graph2) =paste0("V",1:numberofNode)
        write.csv(graph2,paste0('network/Graph2-',numberofNode,'-',DP,'-',rep,'.csv'))
      }
    }
}

## Real Data ##

# Read text file
AT = read.table('/realdata/AT.txt')
CE = read.table('/realdata/CE.txt')
CEAT = read.table('/realdata/CE-AT-geneIds.txt')
dim(AT)
dim(CE)
dim(CEAT)

ppiAT <- data.frame(protein_A = paste0("p", AT[,1]),
                  protein_B = paste0("p", AT[,2]))
ppiCE <- data.frame(protein_A = paste0("p", CE[,1]),
                    protein_B = paste0("p", CE[,2]))
adjAT <- adjacency_matrix_from_data_frame(ppiAT)
adjCE <- adjacency_matrix_from_data_frame(ppiCE)
dim(adjAT)
dim(adjCE)
sum(sum(adjAT))/2
sum(sum(adjCE))/2
write.csv(adjAT,'adjAT.csv')
write.csv(adjCE,'adjCE.csv')
