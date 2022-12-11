# Simulate Networks
library(SpiecEasi)

# Parameters
numberofNodeT <- c(10, 30, 50, 100, 250, 500)
DPT <- c(0.05, 0.1, 0.9)
REPS <- 30

if (!dir.exists("networks")) {
  dir.create("networks")
}

for (i in 1:length(numberofNodeT)) {
  for (j in 1:length(DPT)) {
    for (rep in 1:REPS) {
      set.seed(rep)
      numberofNode <- numberofNodeT[i]
      DP <- DPT[j]
      e <- d <- numberofNode

      # Synthesize the Networks
      G <- SpiecEasi::make_graph("cluster", d, e)
      rownames(G) <- colnames(G) <- paste0("V", 1:numberofNode)
      write.csv(G, paste0("networks/G-", numberofNode, "-", DP, "-", rep, ".csv"))
      e1 <- sum(G)
      k1 <- which(G > 0)
      k10 <- which(G == 0)
      H <- G
      per <- round(sum(H) * DP)
      se1 <- sample(k1, per)
      se10 <- sample(k10, per)
      H[se1] <- 0
      H[se10] <- 1

      H[lower.tri(H)] <- t(H)[lower.tri(H)]
      diag(H) <- 0
      e2 <- sum(H)
      e12 <- sum(G == H) / (e * d)
      rownames(H) <- colnames(H) <- paste0("V", 1:numberofNode)
      write.csv(H, paste0("networks/H-", numberofNode, "-", DP, "-", rep, ".csv"))
    }
  }
}
