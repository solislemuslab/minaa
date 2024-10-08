plot_alignment <- function(
    g_filepath,
    h_filepath,
    alignment_filepath,
    output_filepath,
    st = 0,
    hide_singletons = FALSE
) {
    # # Load necessary libraries
    # required_packages <- c("igraph", "ggraph", "ggplot2", "readr", "dplyr")
    # installed_packages <- rownames(installed.packages())
    # for (pkg in required_packages) {
    #     if (!pkg %in% installed_packages) {
    #         install.packages(pkg, dependencies = TRUE)
    #     }
    # }

    library(igraph)
    library(ggraph)
    library(ggplot2)
    library(readr)
    library(dplyr)

    # Helper function to read adjacency matrix
    read_adjacency <- function(filepath) {
        df <- read_csv(filepath, show_col_types = FALSE)
        mat <- as.matrix(df[, -1])
        rownames(mat) <- df[[1]]
        return(mat)
    }

    # Read adjacency matrices
    g_mat <- read_adjacency(g_filepath)
    h_mat <- read_adjacency(h_filepath)

    # Create igraph objects (assuming undirected graphs; adjust 'mode' if needed)
    g_graph <- graph_from_adjacency_matrix(g_mat, mode = "undirected", diag = FALSE)
    h_graph <- graph_from_adjacency_matrix(h_mat, mode = "undirected", diag = FALSE)

    # Read alignment matrix
    alignment_df <- read_csv(alignment_filepath, show_col_types = FALSE)
    alignment_mat <- as.matrix(alignment_df[, -1])
    rownames(alignment_mat) <- alignment_df[[1]]

    # Extract alignment pairs with similarity > the similarity threshold
    alignment_pairs <- which(alignment_mat > st, arr.ind = TRUE)
    align_df <- data.frame(
        g_node = rownames(alignment_mat)[alignment_pairs[, 1]],
        h_node = colnames(alignment_mat)[alignment_pairs[, 2]],
        similarity = alignment_mat[alignment_pairs]
    )

    if (hide_singletons) {
        # Remove nodes in G and H with degree 0
        deg_g <- degree(g_graph)
        non_single_g <- names(deg_g[deg_g > 0])
        g_graph <- induced_subgraph(g_graph, vids = non_single_g)

        deg_h <- degree(h_graph)
        non_single_h <- names(deg_h[deg_h > 0])
        h_graph <- induced_subgraph(h_graph, vids = non_single_h)

        # Update alignment_df to include only existing nodes after subsetting
        align_df <- align_df %>%
            filter(g_node %in% V(g_graph)$name & h_node %in% V(h_graph)$name)
    }

    # Rename nodes to ensure uniqueness
    V(g_graph)$name <- paste0("G_", V(g_graph)$name)
    V(h_graph)$name <- paste0("H_", V(h_graph)$name)

    combined_graph <- disjoint_union(g_graph, h_graph)

    # Prepare alignment edges
    if (nrow(align_df) > 0) {
        align_edges <- align_df %>%
            mutate(
                from = paste0("G_", g_node),
                to = paste0("H_", h_node)
            )
    } else {
        align_edges <- data.frame(from = character(0), to = character(0), similarity = numeric(0))
    }

    # Create a layout with G on the left and H on the right
    set.seed(123) # For reproducibility
    layout_g <- layout_with_fr(g_graph)
    layout_h <- layout_with_fr(h_graph)

    # Shift H layout to the right of G
    shift_x <- max(layout_g[, 1]) - min(layout_h[, 1]) + 5
    layout_h[, 1] <- layout_h[, 1] + shift_x

    # Combine layouts
    combined_layout <- rbind(layout_g, layout_h)
    V(combined_graph)$x <- combined_layout[, 1]
    V(combined_graph)$y <- combined_layout[, 2]

    # Prepare data for alignment edges
    if (nrow(align_edges) > 0) {
        align_coords <- align_edges %>%
            mutate(
                from_x = V(combined_graph)$x[match(from, V(combined_graph)$name)],
                from_y = V(combined_graph)$y[match(from, V(combined_graph)$name)],
                to_x = V(combined_graph)$x[match(to, V(combined_graph)$name)],
                to_y = V(combined_graph)$y[match(to, V(combined_graph)$name)]
            )
    }

    # Plot #
    
    # Adjust the plot size based on the number of nodes
    num_nodes <- vcount(combined_graph)
    plot_width <- max(2000, 100 + 20 * num_nodes)
    plot_height <- max(1000, 100 + 10 * num_nodes)

    png(filename = output_filepath, width = plot_width, height = plot_height, res = 150)

    p <- ggraph(combined_graph, layout = "manual", x = V(combined_graph)$x, y = V(combined_graph)$y) +
        # Internal edges within G and H
        geom_edge_link(aes(alpha = 0.5), color = "grey", linewidth = 0.5) +
        # G nodes in red, H in blue
        geom_node_point(aes(color = ifelse(startsWith(name, "G_"), "G", "H")), size = 3) +
        # Node labels without the prefix
        geom_node_text(aes(label = gsub("^[GH]_", "", name)), repel = TRUE, size = 3)
        # Color mapping
        scale_color_manual(values = c("G" = "red", "H" = "blue")) +
        theme_void() +
        theme(legend.position = "none")

    if (nrow(align_edges) > 0) {
        # Add alignment edges
        p <- p +
            geom_segment(
                data = align_coords,
                aes(x = from_x, y = from_y, xend = to_x, yend = to_y),
                color = "purple", linewidth = 0.5, alpha = 0.6
            )
    }

    print(p)
    dev.off()

    message("Plot saved to ", output_filepath)
}
