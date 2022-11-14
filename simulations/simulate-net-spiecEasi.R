#rm(list=ls())
library(SpiecEasi)
set.seed(100)
e <- d<-30

#Synthesize the Net
graph1 <- make_graph('cluster', d, e)
write.csv(graph1,"Graph1.csv")
graph2 <- make_graph('cluster', d, e)
write.csv(graph2,"Graph2.csv")
sum(graph1==graph2)/(e*d)
sum(graph1)
sum(graph2)
k1=which(graph1>0)

k2=which (graph2>0)
k10=which(graph1==0)

k20=which(graph2==0)

per=round(sum(graph1)*0.05)
se1= sample(k1,per)
se2= sample(k2,per)

se10= sample(k10,per)
se20= sample(k20,per)

graph1[se1]=0
graph2[se2]=0
graph1[se10]=1
graph2[se20]=1
sum(graph1==graph2)/(e*d)
sum(graph1)
sum(graph2)
write.csv(graph1,"G1.csv", row.names = FALSE)
write.csv(graph2,"G2.csv", row.names = FALSE)