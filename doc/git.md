# `git`

All about `git`.

## General workflow

![General git worflow](git.png)

## Branching model

* `master`: stable, should always pass
* `develop`: development, merge of topic branches, should mostly pass
* `[developer name]`: topic branch, for isolated development

## Clone this repository

```text
git clone https://github.com/richelbilderbeek/djog_unos_2018
```

Then probably, you want to go into that folder:

```text
cd djog_unos_2018
```

## Check the status

```text
git status
```

This will show you which branch you are on, and which files you have changed.

Do this within the `djog_unos_2018` folder.

## Checkout the `develop` branch

```text
git checkout develop
```

Do this within the `djog_unos_2018` folder.

* Note: there was a time when using `git checkout -t origin/develop` was
   recommended. This may have been unnecessarily complex

## Checkout your branch

If your branch is for example `richel`, do:

```text
git checkout richel
```

Do this within the `djog_unos_2018` folder.

* Note: there was a time when using `git checkout -t origin/richel` was
   recommended. This may have been unnecessarily complex

## Update a branch

```text
git pull
```

Do this within the `djog_unos_2018` folder.

## Upload your work to GitHub

```text
git add --all :/
git commit -m "Something brilliant"
git push
```

Do this within the `djog_unos_2018` folder.

Tips and examples:

* Use `progress #123` in the commit message if you worked on/finished Issue 123
* Please do not use `fix #XXX`instead of `progress #XXX` in your commit message, this will prevent the issue from getting closed before being tested
* Use `[skip ci]` at the end of the commit message if Travis does not need to check it

## Merge the other's work with yours

First update `develop`:

```text
git checkout develop
git pull
```

Then merge it with yours:

```text
git checkout richel
git merge develop
git push
```

If you get merge conflicts and just want to use the `develop` version, use:

```text
git checkout richel
git merge -X theirs develop
git push
```

## Merge your work with the other's

* :warning: Only mediors and seniors are allowed to do this!

Go to `develop`:

```text
git checkout develop
git pull
```

`git pull` should not do much! If it does, merge `develop` with your branch first.

If and only if `develop` was already up to date, merge it with yours:

```text
git merge richel
git push
```

## Branching model as a picture

![Branching model](git_branches.png)
