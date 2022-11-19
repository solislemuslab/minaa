#rm(list=ls())
library(SpiecEasi)
########Parameter
numberofNodeT=c(10,30,50)
DPT=c(0.05,0.9)
#####################
Summ=c()
for (i in 1:length(numberofNodeT)){
  for (j in 1:length(DPT)){
    set.seed(100)
    numberofNode=numberofNodeT[i]
    DP=DPT[j]
    e <- d<-numberofNode
    ##numberofNode=numberofEdge
    #Synthesize the Net
    graph1 <- make_graph('cluster', d, e)
    write.csv(graph1,paste('Graph1-',numberofNode,'-',DP,'.csv'), row.names = FALSE)
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
    write.csv(graph2,paste('Graph2-',numberofNode,'-',DP,'.csv'), row.names = FALSE)
    Summ=rbind(Summ,c(numberofNode,DP,per,e1,e2,e12))
  }
}

colnames(Summ) <- c("number of nodes", "percent of diff (% of edges are seleced)", "num of edges to diff", "num of edges in Graph1", "num of edges in Graph2", "similarity(sum(graph1==graph2)/(e*d))")
write.csv(Summ,'Summary.csv')


