Instructions to create a PR in FENIX/MOOSE/etc.

This assumes that you have a working an up to date version of the application.


Create an issue (this can be done ahead of time, or anytime before pushing your changes - However, you will need to list an issue number in one of your commits)
1. Select “New issue” on this page: https://github.com/idaholab/fenix/issues
2. Select either “bug report” or “feature request” (most should be feature requests)
3. Provide issue descriptions of a couple of sentences (or as much as you see fit) following the prompts.
4. Create issue

You will now have an issue number next to the title. We will use it to link the pull request (PR) to the issue.

Before make your changes locally (which would require having a working version of the application), we will need to create a new branch.
1. In your terminal, go to your application `cd ~/projects/fenix`
2. I usually like to make sure I’m on devel: `git checkout devel`
3. Clean and rest devel to get it up to date (make sure you do not have any changes on devel, because you would lose that): `git reset --hard upstream/devel && git submodule update --init`
4. Create new branch: `git checkout -b <BRANCH_NAME>`

At that point, you have a local branch that is up to date with upstream. You’re now ready to make your changes. Remember, small, consistent changes are much better than dumping massive, unrelated changes all at once.
1. Make your changes locally to address the issue (partly or fully) - using VSCode is recommended https://mooseframework.inl.gov/help/development/VSCode.html
2. In your terminal, go to your application `cd ~/projects/fenix`
3. Make sure that the app still compiles and that the tests run: `make -j6 && ./run_tests -j6`. If you modified the documentation, you can build and test it locally: `cd ~/projects/fenix/doc && ./moosedocs.py build --serve`
4. Type `git status` to see the status of your branch. - that should show you the changes files and give you some commands to stage them.
5. Once you are ready, do `git add <FILE_NAME_1> <FILE_NAME_2> <FILE_NAME_3>` to stage your updated files.
6. At that point, I like to run `git clang-format HEAD~1` to fix the format of the files. If files are changed, run `git add <…>` for these files
7. Now you’re ready to commit. Run `git commit`
8. A file will appear, Press the key ‘i’, and you’ll be able to type a short description of the commit. I personally like to have a short title, and then a list of items. At the bottom, you should add `(Ref. #<ISSUE_NUMBER>`. It will look something like that:

Updating this to enable that:
- Update this file to do this
- Update documentation to reflect that change
- Create new test for new capability
- Update existing gold files to reflect change

(Ref. #1234)

9. To exit and save, press ‘esc’, then type ‘:wq`. The commit file will close.
10. You might have to do several commits
11. At some point, you’ll be ready to push your work to your origin: run `git push origin <BRANCH_NAME>
12. A link will be printed in the terminal, go to that
13. You’ll find a page with instructions to create your PR. Fill out the form (I usually copy what I put in my commits message) and submit your PR! You can also tag people in your PR to get their attention.
