## Solving Preppin' Data Weeks 23 and 24 2023 using Enso

In this post, I will walk through solving the [Preppin' Data challenges](https://preppindata.blogspot.com/) for [week 23](https://preppindata.blogspot.com/2023/06/2023-week-23-is-it-teacher-or-student.html) and [week 24](https://preppindata.blogspot.com/2023/06/2023-week-24-is-it-teacher-or-student.html) of 2023. Internally, we have been using challenges like these to help us make the APIs easier to work with and use. 

This walkthrough is based on the current version of Enso which can be downloaded from [here](https://github.com/enso-org/enso/releases/latest). Once installed, the IDE will ask you to sign in on the first use. You can use your GitHub or Google account to sign in or create an account with us. Having signed in, you can then switch to using a local environment by clicking the toggle in the top left. 

![Switching to local environment](./cloud_toggle.png)

Our cloud offering will be publicly available soon. Having switched to the local environment, you can then create a new project by clicking the "New empty project" button just below the environment switch.

![Create a new project](./new_project.png)

Once the project is created it should open automatically. If it doesn't you can click on the 

## Reading in the data

Both of these challenges use the same [dataset](https://docs.google.com/spreadsheets/d/1Sbnu30CXoP3ofUOIGmCqqx1hTTgMokG1/edit?usp=share_link&ouid=113142875990517226017&rtpof=true&sd=true), so first download that and