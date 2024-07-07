plot_alignment <- function(g_filepath, h_filepath, alignment_filepath, output_filepath, th_align, zero_degree, vertex_label_value, size_aligned) {

  # Load adjacency matrices and alignment matrix
  adj_G <- as.matrix(read.csv(g_filepath, header = TRUE, row.names = 1, check.names = FALSE))
  adj_H <- as.matrix(read.csv(h_filepath, header = TRUE, row.names = 1, check.names = FALSE))
  alignment_GH <- as.matrix(read.csv(alignment_filepath, header = TRUE, row.names = 1, check.names = FALSE))

  g_size <- nrow(adj_G)
  h_size <- nrow(adj_H)
  adj_G <- adj_G[1:g_size, 1:g_size]
  adj_H <- adj_H[1:h_size, 1:h_size]
  alignment_GH <- alignment_GH[1:g_size, 1:h_size]

  # Create graph objects with unique vertex names
  graph_G <- graph_from_adjacency_matrix(adj_G, mode = "undirected")
  V(graph_G)$name <- rownames(adj_G)

  graph_H <- graph_from_adjacency_matrix(adj_H, mode = "undirected")
  V(graph_H)$name <- rownames(adj_H)

  # Identify aligned nodes
  alignment_indices <- which(alignment_GH >= th_align, arr.ind = TRUE)
  aligned_nodes_G <- V(graph_G)$name[alignment_indices[, 1]]
  aligned_nodes_H <- V(graph_H)$name[alignment_indices[, 2]]

  # Optionally remove nodes with degree 0, except aligned nodes
  zero_deg_nodes_G <- V(graph_G)[degree(graph_G) < zero_degree & !(V(graph_G)$name %in% aligned_nodes_G)]
  zero_deg_nodes_H <- V(graph_H)[degree(graph_H) < zero_degree & !(V(graph_H)$name %in% aligned_nodes_H)]
  graph_G <- delete.vertices(graph_G, zero_deg_nodes_G)
  graph_H <- delete.vertices(graph_H, zero_deg_nodes_H)

  # Re-calculate the aligned nodes after potential removal
  aligned_indices_G <- match(aligned_nodes_G, V(graph_G)$name)
  aligned_indices_H <- match(aligned_nodes_H, V(graph_H)$name)

  # Filter out NA values from aligned indices
  valid_indices <- which(!is.na(aligned_indices_G) & !is.na(aligned_indices_H))
  aligned_indices_G <- aligned_indices_G[valid_indices]
  aligned_indices_H <- aligned_indices_H[valid_indices]

  # Create a combined graph for alignment visualization
  combined_graph <- graph.disjoint.union(graph_G, graph_H)

  # Define layout for combined graph with circular layout
  layout_G <- layout_in_circle(graph_G)
  layout_H <- layout_in_circle(graph_H)

  png(output_filepath, width = 1600, height = 1200, res = 120)

  # Adjust the layout to place aligned nodes directly opposite each other
  adjust_opposite_positions <- function(layout_G, layout_H, indices_G, indices_H) {
    num_nodes <- length(indices_G)
    angle_step <- pi / (num_nodes + 1)
    start_angle <- pi / 2

    for (i in seq_along(indices_G)) {
      angle <- start_angle - (i * angle_step)
      layout_G[indices_G[i], ] <- c(cos(angle), sin(angle))
      layout_H[indices_H[i], ] <- c(-cos(angle), sin(angle))
    }
    return(list(layout_G, layout_H))
  }

  layouts <- adjust_opposite_positions(layout_G, layout_H, aligned_indices_G, aligned_indices_H)
  layout_G <- layouts[[1]]
  layout_H <- layouts[[2]]

  # Reduce the separation distance between G and H networks
  layout_G <- layout_G - cbind(rep(2, nrow(layout_G)), rep(0, nrow(layout_G)))
  layout_H <- layout_H + cbind(rep(2, nrow(layout_H)), rep(0, nrow(layout_H)))

  # Combine layouts
  layout_combined <- rbind(layout_G, layout_H)

  # Add edges based on alignment matrix GH
  for (i in seq_along(aligned_indices_G)) {
    combined_graph <- add_edges(combined_graph, c(aligned_indices_G[i], aligned_indices_H[i] + vcount(graph_G)))
  }

  # Define colors for vertices and edges
  vertex_colors <- c(rep("lightblue", vcount(graph_G)), rep("lightgreen", vcount(graph_H)))
  vertex_colors[aligned_indices_G] <- "dodgerblue"
  vertex_colors[aligned_indices_H + vcount(graph_G)] <- "green"
  edge_colors <- c(rep("black", ecount(graph_G) + ecount(graph_H)), rep("purple", length(aligned_indices_G)))

  # Define sizes and label sizes for vertices
  vertex_sizes <- rep(1, vcount(combined_graph))
  vertex_sizes[c(aligned_indices_G, aligned_indices_H + vcount(graph_G))] <- size_aligned
  vertex_label_sizes <- rep(vertex_label_value, vcount(combined_graph))
  vertex_label_sizes[c(aligned_indices_G, aligned_indices_H + vcount(graph_G))] <- 0.8

  # Plot the combined graph with alignment edges
  plot(combined_graph,
    layout = layout_combined,
    vertex.label = V(combined_graph)$name, # Add vertex labels
    vertex.size = vertex_sizes, # Adjust vertex size for better visualization
    vertex.label.cex = vertex_label_sizes, # Adjust label size for better readability
    vertex.color = vertex_colors,
    edge.color = edge_colors,
    edge.width = c(rep(1, ecount(graph_G) + ecount(graph_H)), rep(2, length(aligned_indices_G))),
    main = "Result of MiNAA",
    asp = 0 # Set aspect ratio to ensure circular layout
  )

  dev.off()
}