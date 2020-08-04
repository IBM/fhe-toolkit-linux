----
# Contributing to the IBM FHE Toolkit for Linux

Adding new features, improving documentation, fixing bugs, writing new tests, 
designing and coding new examples or writing tutorials are all examples of 
helpful contributions.

## Prerequisites
All the prerequisites for contributions are already installed in every Docker 
container. They include C++ compiler and linker, cmake, clang-format, git tools,
editors, plus the IBM HElib and its dependencies.

If you are new to GitHub, you can find useful documentation [here][1], and 
information on the `git` version control system in this [handbook][2].

## Coding convention
This toolkit comprises a combination of `bash scripts` for building the Docker 
containers and `C++14` for coding the examples, and it uses `clang-format` for 
C++ code formatting. When making code contributions to this toolkit, we ask you 
to follow the [Shell Style Guide][3] for bash scripting and the `C++14` coding 
standard for C++ code. Please remember to format your C++ code using the 
[clang format](.clang-format) style file included in this distribution.

## Steps to contribute 
When contributing to IBM FHE Toolkit for Linux, we ask you to follow some simple steps:
- Create a new git [`issue`][4] describing the type of contribution (new feature,
documentation, bug fix, new tests, new examples, tutorial, etc) and assigning it to 
yourself, this will help to inform others that you are working on the contribution 
and if you want, it is also a vehicle to ask for help with it.
- Create your own git [`fork`][5] of this repository to work on the intended contribution.
- Once the contribution is complete, tested, documented and ready for review by 
the committer team, please initiate a GitHub [`pull request`][6] into the `master` branch 
from **your fork of the repository**.

## Signing off your contribution
This project uses [DCO][7]. Be sure to [sign off][8] your contributions using 
the `-s` flag or adding `Signed-off-By: Name<Email>` in the git commit message.
We will not be able to accept non-signed contributions.

### Example commit message
```bash
git commit -s -m 'Informative commit message'
```

  [1]: https://docs.github.com/en/github/using-git    "GitHubDocs"
  [2]: https://guides.github.com/introduction/git-handbook/    "gitHandbook"
  [3]: https://google.github.io/styleguide/shellguide.html    "ShellStyle"
  [4]: https://docs.github.com/en/github/managing-your-work-on-github/managing-your-work-with-issues   "gitIssue"
  [5]: https://docs.github.com/en/github/getting-started-with-github/fork-a-repo    "gitFork"
  [6]: https://docs.github.com/en/github/collaborating-with-issues-and-pull-requests/creating-a-pull-request-from-a-fork    "GitHubPullReq"    
  [7]: https://developercertificate.org/    "DCO"
  [8]: https://docs.github.com/en/github/authenticating-to-github/signing-commits    "gitSignoff"
