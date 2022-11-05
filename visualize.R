# # if not installed, install igraph
# list_of_packages <- c("igraph")
# new_packages <- list_of_packages[!(list_of_packages %in% installed.packages()[,"Package"])]
# if(length(new_packages) > 0)
# {
#   install.packages(new_packages)
# }
install.packages("igraph") # all we need for now
library(igraph)

# get specified file name
args <- commandArgs(TRUE)

# Check arg count
if (length(args) != 1)
{
  stop("Invalid number of arguments.", call.=FALSE)
}
infile <- args[1]

graph <- read.csv(infile)

# Things for colapse:
E(graph)$weight = runif(ecount(graph))
c_scale <- colorRamp(c('red','blue','purple'))
E(graph)$color = apply(c_scale(E(graph)$weight), 1, function(x) rgb(x[1]/255, x[2]/255, x[3]/255))

plot(graph, vertex.size=10, vertex.label.dist=1.5)
