# `git`

All about `git`.

## Branching model

 * `master`: stable, should always pass
 * `develop`: development, merge of topic branches, should mostly pass
 * `[developer name]`: topic branch, for isolated development

![Branching model](git_branches.png)

## Branches

Name|Branch name
---|---
@annabelliard|`annabel`
@annehinrichs22|`anne`
@EllyJet |`?`
@Joshua260403 |`joshua`
@Martje127 |`mart`
@RafayelGardishyan|`rafayel`
@richelbilderbeek|`richel`
@Rijk-van-Putten|`rijk`
@Same-Drenth|`same`

## Clone this repository

```
git clone https://github.com/richelbilderbeek/djog_unos_2018
```

Then probably, you want to go into that folder:

```
cd djog_unos_2018
```

## Check the status

```
git status
```

This will show you which branch you are on, and which files you have changed.

## Checkout the `develop` branch for the first time

```
git checkout -t origin/develop
```

Do this within the `djog_unos_2018` folder.

## Checkout your branch for the first time

If your branch is for example `richel`, do:

```
git checkout -t origin/richel
```

Do this within the `djog_unos_2018` folder.

## Checkout the `develop` branch again

```
git checkout develop
```

Do this within the `djog_unos_2018` folder.

## Checkout your branch again

If your branch is for example `richel`, do:

```
git checkout richel
```

Do this within the `djog_unos_2018` folder.

## Update a branch

```
git pull
```

Do this within the `djog_unos_2018` folder.

## Upload your work to GitHub

```
git add --all :/
git commit -m "Something brilliant"
git push
```

Do this within the `djog_unos_2018` folder.

Tips:

 * Use `fix #123` in the commit message if it fixed Issue 123
 * Use `[skip ci]` at the end of the commit message if Travis does not need to check it

## Merge the other's work with yours

First update `develop`:

```
git checkout -t origin/develop
git pull
```

Then merge it with yours:

```
git checkout -t origin/richel
git merge develop
git push
```

## Merge your work with the other's

Go to `develop`:

```
git checkout -t origin/develop
git pull
```

`git pull` should not do much! If it does, merge `develop` with your branch first.

If and only if `develop` was already up to date, merge it with yours:

```
git merge richel
git push
```
