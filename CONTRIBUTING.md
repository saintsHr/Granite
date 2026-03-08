# Contributing

Hey! Thanks for your interest in contributing to this project. I made this document to help everyone who wants to contribute get some guidance. Here's a quick guide for you.

---

## Planning

First of all, you should decide **how** you are going to contribute (feature, bug, docs, etc.). We have a few tags you should choose and follow:

- feature
- bug
- documentation
- improvement
- optimization
- refactor

Once you decide **how** you are contributing, you should choose **where**. For that, we have some tags too:

- Core
- Renderer
- Scene
- Window
- Physics
- Input
- Assets

---

## Forking

Now that you have decided everything, you should create your own copy of the repository. This is called a **fork**. In this fork, you can make your changes freely without interfering with the original repo. To do so, follow these steps:

- Go to the original repository: https://github.com/saintsHr/Granite
- On the top-right side of the repository, click the **Fork** button
- Select the account you want to fork the repository to

Now you should have an exact copy of the official repository in your account!

---

## Making Your Changes

From now on, you can make any changes you want to your fork, such as adding features, fixing bugs, optimizing code, and more! It is **highly recommended** that you make **one focused change at a time**, as not doing so could lead to your contribution not being accepted. After you have made your changes, follow the next step.

---

## Pull Request

Once you have finished modifying and testing, you should create a **Pull Request**, which is a way of saying: *"Hey, I did some cool things on my fork and I’d like to merge them into your repository. Can I?"*  

Follow these steps:

1. Create a branch on your local repository following this template:  

```git checkout -b <type>/<module>/<title>```

Example:  

```git checkout -b feature/renderer/added-shadows```


2. Add your changes, commit, and push:  

```
git add .
git commit -m "type(module): title"
git push origin type/module/title
```

Example:  

```
git add .
git commit -m "feat(renderer): added shadows"
git push origin feature/renderer/added-shadows
```

3. On the original repository page:  
- Go to the **Pull requests** tab  
- Press the **New pull request** button  
- Press **compare across forks**  
- Set **base repository** as `saintsHr/Granite` and **base branch** as `main`  
- Set **head repository** as `(YourUsername)/Granite` and **compare** as your branch  
- Write a clear and complete title & description  
- Press **Create pull request**

---

Done! If you followed these steps correctly, your pull request will be reviewed, tested, and merged. Thank you so much for taking the time to read this and contribute. Cya!
