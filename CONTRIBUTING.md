# How To Contribute

If you want to add a feature first open an issue so we can talk about it.  
Other than that feel free to tackle one of the issues.

## Branching Model

The project uses [Driessen's branching model](https://nvie.com/posts/a-successful-git-branching-model/)

### How Does It Work

Basically, fork the project on github, clone, make a new branch:

```bash
git clone <url_to_your_fork>
cd among-stars
git checkout -b <branch_name> develop
```

After changing some files, you can test your changes and catch any errors by [building](/README.md#compiling) the project.  
Then commit and push:

```bash
# made some changes
make
git commit -m 'commit message'
git push origin <branch_name>
```

You can now open a pull request from `<branch_name>` to the `develop` branch. Thanks for your contribution! 🤟
