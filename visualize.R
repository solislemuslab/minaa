# # if not installed, install igraph
# list_of_packages <- c("igraph")
# new_packages <- list_of_packages[!(list_of_packages %in% installed.packages()[,"Package"])]
# if(length(new_packages) > 0)
# {
#   install.packages(new_packages)
# }
#install.packages("igraph", repos = "http://cran.us.r-project.org") # all we need for now
library(igraph)

# get specified file name
args <- commandArgs(TRUE)

# Check arg count
if (length(args) != 2)
{
  stop("Invalid number of arguments.", call.=FALSE)
}
infile <- args[1]
outfile <- args[2]
outpath <- paste0(outfile, "merged.jpeg")

# Read csv to graph object
data <- read.csv(infile, header=TRUE, quote = "\"", row.names=1, check.names=FALSE, na.strings = "")
matrix <- as.matrix(data)
g <- graph.adjacency(matrix, mode="undirected", weighted=TRUE)
g <- simplify(g)
l <- layout.sphere(g)

# Color Vertices
# uhhh

# Color Edges
E(g)$weight = E(g)$weight / 3
c_scale <- colorRamp(c('red','blue'))
E(g)$color = apply(c_scale(E(g)$weight), 1, function(x) rgb(x[1]/255, x[2]/255, x[3]/255))

# svg(file="merged.svg", width=20, height=20)
jpeg(file=outpath, width=2000, height=2000)
plot(g, layout=l, vertex.label.dist = 1, vertex.size=3, edge.width=1.5) #vertex.color="red"

dev.off()