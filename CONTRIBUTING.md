# Contributing

## Branching policy

* The `main` branch should always build successfully
* The `development` branch is for developers and should almost always build successfully
* The other branches are topic branches

## `git` usage

### Clone the repo

To get started working on `djog_unos_2018` do:

```text
git clone https://github.com/richelbilderbeek/djog_unos_2018
```

### Switch to your topic branch

You will do most development on the `develop` branch and your topic branch.

Checkout the `develop` branch and your topic branch (here: `richel)`.
This needs to be done once:

```text
git checkout -t origin/develop
git checkout -t origin/richel
```

### Push your work to GitHub

The workflow is the common `git` workflow:

```text
git add --all :/
git commit -m "Did something awesome"
git push
```

If this gives a longer error message stating you need to do a pull request, do so:

```text
git pull
```

... then do the add, commit and push.


### Update your topic branch

Start a day by updating your topic branch with the `develop` branch:

```text
git checkout develop
git pull
git checkout richel
git merge develop
```

If this passes silently, all you need to do is push:

```text
git push
```

If you get merge conflicts, fix these and do a add, commit and push.

### Merge you work to `develop`

If and only if your build passes, merge it with `develop`:

```text
git checkout develop
git pull
git merge richel
git push
```

After this, merge `develop` back to your own topic branch again:

```text
git checkout richel
git merge develop
git push
```

### Merge with `main`

If and only if the `develop` build passes, merge it with `main`:

```text
git checkout main
git merge develop
```

Only @richelbilderbeek and @RafayelGardishyan do this.
