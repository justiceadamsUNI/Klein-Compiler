 $$$$$$\   $$$$$$\  $$\      $$\ $$$$$$$\ $$$$$$\ $$\       $$$$$$$$\ $$$$$$$\  
$$  __$$\ $$  __$$\ $$$\    $$$ |$$  __$$\\_$$  _|$$ |      $$  _____|$$  __$$\ 
$$ /  \__|$$ /  $$ |$$$$\  $$$$ |$$ |  $$ | $$ |  $$ |      $$ |      $$ |  $$ |
$$ |      $$ |  $$ |$$\$$\$$ $$ |$$$$$$$  | $$ |  $$ |      $$$$$\    $$$$$$$  |
$$ |      $$ |  $$ |$$ \$$$  $$ |$$  ____/  $$ |  $$ |      $$  __|   $$  __$$< 
$$ |  $$\ $$ |  $$ |$$ |\$  /$$ |$$ |       $$ |  $$ |      $$ |      $$ |  $$ |
\$$$$$$  | $$$$$$  |$$ | \_/ $$ |$$ |     $$$$$$\ $$$$$$$$\ $$$$$$$$\ $$ |  $$ |
 \______/  \______/ \__|     \__|\__|     \______|\________|\________|\__|  \__|
                                                                                
                                                                                
                                                                                
$$$$$$$\  $$$$$$$$\ $$$$$$$$\ $$$$$$\ $$\       $$$$$$$$\ $$$$$$$\   $$$$$$\    
$$  __$$\ $$  _____|$$  _____|\_$$  _|$$ |      $$  _____|$$  __$$\ $$  __$$\   
$$ |  $$ |$$ |      $$ |        $$ |  $$ |      $$ |      $$ |  $$ |$$ /  \__|  
$$ |  $$ |$$$$$\    $$$$$\      $$ |  $$ |      $$$$$\    $$$$$$$  |\$$$$$$\    
$$ |  $$ |$$  __|   $$  __|     $$ |  $$ |      $$  __|   $$  __$$<  \____$$\   
$$ |  $$ |$$ |      $$ |        $$ |  $$ |      $$ |      $$ |  $$ |$$\   $$ |  
$$$$$$$  |$$$$$$$$\ $$ |      $$$$$$\ $$$$$$$$\ $$$$$$$$\ $$ |  $$ |\$$$$$$  |  
\_______/ \________|\__|      \______|\________|\________|\__|  \__| \______/ 
-----------------------------------------------------------------------------------------


Hello, we are the compiler defilers. Our Group Consist of 
	- Justice Adams (adamsjal@uni.edu)
	- Harsha Varma (varmak@uni.edu)
	- Joshua Hilliard (hilliarj@uni.edu)


This is the code repository for the Klein Compiler we are writing. We are implementing a
Klein compiler that will translate .kln files into machine language. This compiler
implementation is written in C++. Currently we have a working Scanner which converts
characters in a .kln file into tokens. We developed this using a feature-branch workflow
utilizing github. View the repo here: (https://github.com/justiceadamsUNI/Klein-Compiler)
You'll notice the repo has an extra outer directory for various reasons including
continous integration purposes.

For a much prettier README, I encourage you to check out the Github repo.

# A quick note about Visual Studio. If you don't intend to edit this using visual studio, ignore
the Compiler.sln , Compiler.vcxproj	,Compiler.vcxproj.filters files. If you are, they will
make your life easier!


PROJECT STRUCTURE:
-----------------------------------------------------------------------------------------
Our project is structured like the following
	Compiler(root directory)
	|
	|----- src (Source Files)
	|		|
	|		|__ header (Header C++ files for main compiler code files)
	|		|
	|		|__ implementation (The implementation of the  C++ header files in /header)
	|
	|----- test (Files for testing source files)
	|
	|----- programs (Valid klein files written in klein programming language)
	|
	|----- docs (Files for document our process of building this project)
	|
	|----- misc (Any other project files we feel are relevant)
	
	
	SRC: A directory containing all code for the project. Header files can be found in /header
	and implementation files of those headers found in /implementation. You'll notice 
	Compiler.cpp and BuildTokens.cpp in the src/ directory. They serve seperate purposes and 
	are thus seperated. Compiler.cpp will be our final cpp class containing all other objects
	and BuildTokens.cpp prints the tokens of a valid klein file to the screen.
	
	TEST: A directory containing all files for testing our code. This includes unit test and 
	arbitrary (invalid) .kln files for further testing.
	
	PROGRAMS: A collection of valid .kln programs we have written while building this compiler.
	
	DOCS: A collection of documents we've used while building this compiler. Includes language
	grammer as well as any design docs.
	
	MISC: Anything else that doesn't fit in an above directory that we feel is 
	valuable for you to see!
	
	
TESTING:
-----------------------------------------------------------------------------------------
We decided to use CATCH, an open source C++ testing framework that's lightweight.
IDE-independent, and fully featured. Their repo descripes it as 
'A modern, C++-native, header-only, test framework for unit-tests, TDD and BDD'. Note that
the catch.h is not ours, we're just using it to test! It's a header-only framework.

To run our test you can compile  test/*.cpp and src/implementation/*.cpp into an 
excecutable and run it. All you need is a c++ compiler! For Example: Using a visual studio
dev console you could run 

cl /FeRunTest /EHsc test/*.cpp src/implementation/*.cpp

Then run the RunTest excecutable with RunTest.exe .There you have it.

ALTERNATIVELY: you can run the bash script if you have a g++ compiler and bash :)
We have verified that this works on STUDENT.CS.UNI.EDU server. Run it with

chmod -x runtest.sh

and

./runtest.sh


BUILD SCRIPTS:
-----------------------------------------------------------------------------------------
We will have a collection of build scripts set up to do various things throughout the 
course of this project. Right now we just have kleins. This script will print out
all the valid tokens of any .kln file you pass it as a paramater. You could run this 
bash script with

chmod -x kleins.sh

and

./kleins test_file_path.kln

We have verified that this works on STUDENT.CS.UNI.EDU server.


RESOURCES THAT MADE THIS PROJECT POSSIBLE (Descriptions as listed online):
-----------------------------------------------------------------------------------------
- CATCH: A modern, C++-native, header-only, test framework for unit-tests, TDD and BDD
		(https://github.com/philsquared/Catch)
		
- GIT: Git is a free and open source distributed version control system designed to 
		handle everything from small to very large projects with speed and efficiency
		(https://git-scm.com/)
		
- GITHUB: GitHub is a web-based Git or version control repository and Internet hosting 
		service. It is mostly used for code. (wikipedia desc.)
		(https://github.com/)

- TRAVIS CI: Travis CI is a hosted, distributed continuous integration service used to 
		build and test software projects hosted at GitHub (wikipedia desc.)
		(https://travis-ci.com/)
		
- VISUAL STUDIO: Visual Studio is a Fully-featured integrated development 
		environment (IDE)
		for Android, iOS, Windows, web, and cloud (wikipedia desc.)
		(https://www.visualstudio.com/)


KNOWN BUGS:
-----------------------------------------------------------------------------------------
We have no bugs to report *as of right now*. Let us hope we keep it that way