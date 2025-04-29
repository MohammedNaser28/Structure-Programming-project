# Structure-Programming-project

> [!NOTE]
> ## Description
> I will write the description after mid


> [!NOTE]
> أي حاجة جمبها #main فمعناه إنه شىء أساسي الدكتور طلباه وبدونه هننقص درجات
> أي حاجة جمبها #إضافة فمعناه إنه شيء إضافي أخذنا موافقة الدكتور عليه وعليه كمان درجات زيادة حلوة إن عملناه فعليه درجات وهناخدها إن ماعملنهوش مش هننقص 
> أي حاجة جمبها #إضافة_مختلف_عليها فمعناه شىء إضافي ما أخذناش موافقة الدكتور عليه، ومش هنعمله دلوقتي وممكن مانعملوش أصلا لكنه يظل كاقتراح إن قدر حد صاحبه يعمله أو اتبقى وقت نعمله 
> > [!IMPORTANT]
> > ## البروجكت الشغل عليه هيكون بالأولوية، بمعنى ؟
> > اللي عليه #main فهو اللي هننقص عليه فضروري يكون أول حاجة نعمله ومش هنزق نفسنا في حاجات زيادة قبل ما نخلصه
> > اللي جمبه #إضافة يجي بعد ال #main وهكذا 
> > ## الشغل بردك فيه توازي بمعنى ؟
> > طالما معاك تاسك فانت بتشتغل عليها لوحدك ، مش مطلوب منك تستنى غيرك أو غيرك يستناك كل حاجة اترتبت إننا نشتغل بشكل متوازي ، اللي خلص التاسك ال #main يخش في ال #إضافة  


## Log in & Create Account 
>[!IMPORTANT]
>#### There's no addition function in the Login 
> ##### This mean every thing was written is important and a necessary 

- ### GUI
	- [ ] link to widget to add new user
	- [ ] two input fields for (username and Password)
	- [ ] function to check from user inputs valid
	- [ ] function to close and open the main window
	- [ ] button for log in function
	- [ ] button for log out 
- ### Core
	- [ ] functions to handle filesystem
		- [ ] functions to load filesystem of users and admin information 
		- [ ] function to add file system of user and admin information
	>  we need functions to create and load  file system that stored users information like password and emails, Also we need to think what the format that we will use to store in file system (شكل الداتا هيكون متنظم ازاي عشان لما نرجع نحطها في ال array) 
	- [ ] Function to check from user and password and detect he is admin or user.
	> Function must return something like Boolean value to make system know that is really user and can enter  to  MainWindow  widget
	- [ ] Function to create new user 
	      This function is just a function add new user using file system function
	     - [ ] Function to compare two passwords that user add to check it the same
	     - [ ] Function to check that user name is valid not repeated or use invalid chars
	- [ ] Function to log out from system #main 

> [!NOTE] 
> We can discuss when we can load recipe management data after login function return true or during login 


## Main Window Widget
- ### GUI 
	- [ ]  side section for favorites #أساسية and `editions` #إضافة_مختلف_عليها and my recipe 
		> we can here think about add also search to be in the side panel not in top
	- [ ] make every recipe clickable so when click widget change/open to display recipe
		#### Recipe Page Details 
		- [ ] Image of the page at the top of page 
		- [ ] line contain 
		    - number of ingredients 
		    - level of difficult  
		- [ ] short text that describe the recipe 
		- [ ] the ingredients line by line or as a grid 
			- [ ] name of ingredients and the مقدار+
		- [ ] the steps to cock this recipe 
		- [ ] rate the recipe 
	- [ ] under the menu bar we will add sort #إضافة_مختلف_عليها 
	     - [ ] sort will be a dropdown menu has many options to order the  recipes    
		     - [ ] sort with alphabetic 
		     - [ ] sort with 'time need to cock 
		     - [ ] sort with number of ingredients 
	- [ ]  Search input field #main 
		> Search input will be always visible always  in main widgets 
	- [ ] Push Button to add new recipe #admin_main #user_addition
	- [ ] Push Button to delete recipe #admin_main #user_addition
	- [ ] Push Button to edit recipe #admin_main #user_addition
> [!description]
> Ok we can make another thing 3 dots when click on it it give you two options 
> - Edit
>  - Delete
>    > [!NOTE]
> > It maybe be difficult for gui team
	
- [ ] dynamic widget to change content in widget in search and suggestion    
### Core
	
#### Display recipes & Action on it

 - [ ] Function to suggest recipes #main
 > [!DESCRIPTIAN]
	 > ok This recipes we must return it as array 
	 > The Dynamic of suggestion depends on randomize and some rate from us  

- [ ] Function to rate recipe #إضافة
- [ ] Function to edit recipe for user and be for his own #إضافة 
- [ ] Function to display categorized functions #إضافة 
- [ ] function to search into the array that contain titles of recipe #main 
	- [ ] make the search not sensitive case and ignore spaces 
	- [ ] we must make search in ingredients also
	- [ ] in time need to cook
	- [ ] Difficult of The recipe 
- [ ] function to sort recipes ( ok so we can sort recipes ) #إضافة_مختلف_عليها 
		- [ ] this function take arguments to (time, alpha, number of ingredients, difficulty  )
			- [ ] ok maybe we can make this is Boolean values default is false
			- [ ] maybe we can use overwrite function (the same logic and name just the arguments is different)
		> the sort function is  a return  function we must return an array sorted 
- [ ] Function to suggest random recipe From all recipes in the system (random choice) #إضافة_مختلف_عليها 
 >[!NOTE]
>we can make this function not return the all recipe maybe it can just return an id or something unique that can be call [!JUSTIDEA] 

#### Favorite Tab
- [ ] Function to add favorite tab #main
 - [ ] Function to remove from favorite tab #main
- [ ] Function to display the favorite recipe #main
---


> [!IMPORTANT]
> ## Admin Functionality
>  - [ ] Function to display recipes 
>  > Ok This function is copy from the user functions Just some of edit if it need
>   - [ ] Function to add recipe 
>        > Also this function is similar add favorite just some edit to make it add for all system not  user
>  - [ ] Function to delete recipe 
> 	   > Also this function is similar to delete favorite just some edit to make it delete for all system not user
>  - [ ] Function to edit recipe 
>      >  This is unique function in main functionality 


### Q&A

> [!IMPORTANT]
> 
>### إحنا كده مزودين حاجات على نفسنا وشايف الصراحة إن الموضوع مش ضروري وكفاية طلبات الدكتور
> الfunctions الزيادة هي تقريبا أربعة functions وده مش إجباري حد ياخدهم لو مش عايز  ومش هتتوزع  إلا على اللي عايز ياخدها 
> ### الكلام كتير وال functions كتير 
> لو خدت الموضوع إنه على شخص واحد هيعمل كل ده فعندك حق، لكن لو بصيت إن 70% من الكلام الكتير هيتقسم على 7 هتشوف إنه قليل
> الفايال ده اتعمل عشان اللي حابب يفهم السيستم كله ماشي ازاي يبص عليه 
> ### إيه لازمة اني افهم السيستم كله ماشي ازاي كفاية الجزء بتاعي ؟
> فيه درجات كلنا هناخدها بشكل جماعي كتيم لما نخلص البروجكت وفيه درجات بتتحسب لكل شخص بشكل منفرد وده الشخص هو اللي بيتحاسب عليه من ضمنها إنك تكون فاهم السيستم اللي انت كنت شغال فيه شغال ازاي مش مجرد كتبت كام سطر كود ومشيت 
> وكمان الفايال ده اتعمل عشان نعرف نفصل السيستم ونجيب كل تفاصيله مش أكتر

## Team Members

Khaled 

Shahd Ahmed Anas

Mohammad Abdel-Qawi 

Mohammed Eid

Mohammed Hamed

Mohammed Naser

Mohammed Yasser
