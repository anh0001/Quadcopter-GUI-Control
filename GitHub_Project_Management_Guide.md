# GitHub Project Management Guide for Students

## Step 1: Forking the Repository
1. **Navigate to the Original Repository:** Go to the GitHub page of the original repository you need to work on.
2. **Fork the Repository:** Click on the 'Fork' button located at the top right corner of the page. This creates a copy of the repository in your GitHub account.

## Step 2: Cloning the Forked Repository
1. **Copy the URL of Your Fork:** On your fork's GitHub page, click the 'Code' button and copy the URL provided.
2. **Clone the Repository:**
   - Open your terminal (Command Prompt, PowerShell, Terminal, etc.).
   - Navigate to the directory where you want to clone the repository.
   - Run the command: `git clone [URL of the forked repository]`. Replace `[URL of the forked repository]` with the URL you copied.
   - This command creates a local copy of the forked repository on your computer.

## Step 3: Making Changes
1. **Navigate to the Repository Folder:** Change your directory to the cloned repository by using `cd [repository name]`.
2. **Create a New Branch:** Run `git checkout -b [new-branch-name]`. Replace `[new-branch-name]` with a meaningful name for your new branch.
3. **Make Your Changes:** Edit, add, or delete files as needed for your project.

## Step 4: Committing Changes
1. **Add Changes to the Commit:** Run `git add .` to add all new or modified files to the staging area.
2. **Commit Your Changes:** Run `git commit -m "[commit message]"`. Replace `[commit message]` with a brief description of your changes.

## Step 5: Pushing Changes to GitHub
1. **Push the Branch to Your Fork:** Run `git push origin [new-branch-name]`.

## Step 6: Creating a Pull Request
1. **Navigate to Your Fork on GitHub:** Go to the GitHub page of your fork.
2. **Start a Pull Request:** Click on 'Pull Request' and then on 'New Pull Request'.
3. **Choose the Correct Branch:** Select your newly pushed branch.
4. **Create the Pull Request:** Click on 'Create Pull Request'. Provide a title and description for your changes. Finally, submit the pull request.

## Step 7: Waiting for Review
- After submitting the pull request, wait for the project maintainer to review your changes. They might merge your changes into the original repository or ask for further modifications.

_Reminder_: Frequently sync your fork with the original repository to keep it up-to-date. Regularly commit your changes and communicate effectively through GitHub's commenting systems on pull requests and commits.
