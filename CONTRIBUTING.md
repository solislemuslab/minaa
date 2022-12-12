# Contributing to MiNAA

The following guidelines are designed for contributors to `MiNAA`.

## Reporting Issues and Questions

For reporting a bug, a failed function or requesting a new feature, you can simply open an issue in [the issue tracker](https://github.com/solislemuslab/minaa/issues).
First, seach through existing issues (open or closed) that might have the answer to your question.
If you are reporting a bug, please also include a minimal code example or all relevant information for us to replicate the issue.

## Contributing Code

To make contributions to `MiNAA`, you need to set up your [GitHub](https://github.com/) account (if you do not have one) and request your change(s) or contribution(s) via a pull request against the `main` branch of `MiNAA` from a non-main branch in your fork.
Using a non-main branch on your end will give developers push access to your
branch to make edits to it (in case we want to work collaboratively on the new code).

Please use the following steps:

1. Fork the `MiNAA` repository to your GitHub account
2. Clone your fork locally with `git clone`
3. Create a new branch with a name that describes your contribution. For example, if your contribution is fixing a bug in `readTopology`, your new branch can be named `fix-bug-readTopology`. You can create and switch to it with:

   ```shell
   git checkout -b fix-bug-readTopology
   ```

4. Make your changes on this new branch.
5. Push your changes to your fork.
6. [Submit a pull request](https://github.com/solislemuslab/minaa/pulls) against the `main` branch in `MiNAA`. Make sure that your code passes all the automatic tests and that it is not in conflict with the current status of `main`
