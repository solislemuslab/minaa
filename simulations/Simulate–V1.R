library(SpiecEasi) # make_graph and graph2prec function.
set.seed(10010)

data(amgut1.filt)
depths <- rowSums(amgut1.filt)
amgut1.filt.n  <- t(apply(amgut1.filt, 1, norm_to_total))
amgut1.filt.cs <- round(amgut1.filt.n * min(depths))

d <- ncol(amgut1.filt.cs)
n <- nrow(amgut1.filt.cs)
e <- d

# Synthesize the data
graph <- make_graph('cluster', d, e)

Prec  <- graph2prec(graph)
Cor   <- cov2cor(prec2cov(Prec))
X <- synth_comm_from_counts(amgut1.filt.cs, mar=2, distr='zinegbin', Sigma=Cor, n=n)

# Synthesize the data
graph <- make_graph('band', d, e)

Prec  <- graph2prec(graph)
Cor   <- cov2cor(prec2cov(Prec))
X <- synth_comm_from_counts(amgut1.filt.cs, mar=2, distr='zinegbin', Sigma=Cor, n=n)

# Synthesize the data
graph <- make_graph('scale_free', d, e)

Prec  <- graph2prec(graph)
Cor   <- cov2cor(prec2cov(Prec))
X <- synth_comm_from_counts(amgut1.filt.cs, mar=2, distr='zinegbin', Sigma=Cor, n=n)
