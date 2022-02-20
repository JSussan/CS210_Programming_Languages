/*
 *		Author:	Jonathan Sussan
 *		Date: 	December 9, 2020
 *		Title: 	Project 3: Wildlife Zoo Interface
 *		Class: 	SNHU CS 210
 */

#include <iostream>			// standard input/output streams library
#include <jni.h>			// allows java to be called
#include <vector>			// allows the use of vectors in code
#include <fstream>			// allows both ofstream and ifstream
#include <string>			// allows the use of strings
#include <memory>			// dynamic memory management
#include <typeinfo>			// for the use of typeid operator
using namespace std;

void GenerateData() {		//DO NOT TOUCH CODE IN THIS METHOD
JavaVM *jvm; // Pointer to the JVM (Java Virtual Machine)
JNIEnv *env; // Pointer to native interface
//================== prepare loading of Java VM ============================
JavaVMInitArgs vm_args; // Initialization arguments
JavaVMOption* options = new JavaVMOption[1]; // JVM invocation options
options[0].optionString = (char*) "-Djava.class.path="; // where to find java .class
vm_args.version = JNI_VERSION_1_6; // minimum Java version
vm_args.nOptions = 1; // number of options
vm_args.options = options;
vm_args.ignoreUnrecognized = false; // invalid options make the JVM init fail
//=============== load and initialize Java VM and JNI interface =============
jint rc = JNI_CreateJavaVM(&jvm, (void**)&env, &vm_args); // YES !!
delete options; // we then no longer need the initialization options.
if (rc != JNI_OK) {
// TO DO: error processing...
cin.get();
exit(EXIT_FAILURE);
}
//=============== Display JVM version =======================================
cout << "JVM load succeeded: Version ";
jint ver = env->GetVersion();
cout << ((ver >> 16) & 0x0f) << "." << (ver & 0x0f) << endl;

jclass cls2 = env->FindClass("ZooFileWriter"); // try to find the class
if (cls2 == nullptr) {
cerr << "ERROR: class not found !";
}
else { // if class found, continue
cout << "Class MyTest found" << endl;
jmethodID mid = env->GetStaticMethodID(cls2, "createZooFile", "()V"); // find method
if (mid == nullptr)
cerr << "ERROR: method void createZooFile() not found !" << endl;
else {
env->CallStaticVoidMethod(cls2, mid); // call method
cout << endl;
}
}
jvm->DestroyJavaVM();
cin.get();
}

// use string trim to remove leading and trailing spaces
string trim(string trimMe) {
	// removes all leading spaces
	while (trimMe.size() && isspace(trimMe.front()))
		trimMe.erase(trimMe.begin());
	// remove all trailing spaces
	while(trimMe.size() && isspace(trimMe.back()))
		trimMe.pop_back();
	return trimMe;
}
// use structure for animals to hold member data (structure can hold multiple data types)
struct Animal {
	string Name;
	int TrackNum;
	Animal(string animalName, int TrackNum): Name(animalName), TrackNum(TrackNum){}
	virtual ~Animal(){}
};
struct Oviparous: public Animal {
	int NumberOfEggs;
	Oviparous(string animalName, int TrackNum, int NumberOfEggs): Animal(animalName, TrackNum), NumberOfEggs(NumberOfEggs){}
};
struct Crocodile: public Oviparous{
   Crocodile(string animalName, int TrackNum, int NumberOfEggs): Oviparous(animalName, TrackNum, NumberOfEggs){}
};
struct Goose: public Oviparous{
   Goose(string animalName, int TrackNum, int NumberOfEggs): Oviparous(animalName, TrackNum, NumberOfEggs){}
};
struct Pelican: public Oviparous{
   Pelican(string animalName, int TrackNum, int NumberOfEggs): Oviparous(animalName, TrackNum, NumberOfEggs){}
};

struct Mammal: public Animal{
   int Nurse;
   Mammal(string animalName, int TrackNum, int Nurse): Animal(animalName, TrackNum), Nurse(Nurse){}
};
struct Bat: public Mammal{
   Bat(string animalName, int TrackNum, int Nurse): Mammal(animalName, TrackNum, Nurse){}
};
struct Whale: public Mammal{
   Whale(string animalName, int TrackNum, int Nurse): Mammal(animalName, TrackNum, Nurse){}
};
struct SeaLion: public Mammal{
   SeaLion(string animalName, int TrackNum, int Nurse): Mammal(animalName, TrackNum, Nurse){}
};

// global zoo vector that uses a smart pointer to own and manage another object
vector<unique_ptr<Animal>> WildLifeZoo;

// functions for animals
void AddAnimal() {			// allows user to add data for a new animal
	string trackNum;		// string variable to store track number
	string animalName;		// string variable to store animal name
	string animalType;		// string variable to store animal type
	string subType;			// string variable to store animal sub-type
	string numEggs;			// string variable to store number of eggs
	string isNursing;		// string variable to store if animal is nursing
	char save;

	cout << "Enter Track Number:" << endl;
	cin >> trackNum;			// user input for track number
	cout << "Enter Animal Name:" << endl;
	cin.ignore();
	getline(cin, animalName);	// user input for animal name
	cout << "Enter Type (Mammal or Oviparous):" << endl;
	cin >> animalType;			// user input for animal type
	cout << "Enter Sub-type:" << endl;
	cin >> subType;				// user input for animal sub-type
	cout << "Enter number of Eggs:" << endl;
	cin >> numEggs;				// user input for number of eggs
	cout << "Enter if Nursing (1 for yes, 0 for no):" << endl;
	cin >> isNursing;			// user input for if animal is nursing

	// display user input to confirm if user would like to save data to zoodata.txt file
	cout << trackNum << " " << animalName << " " << animalType << " " << subType << " " << numEggs << " " << isNursing << endl;
	cout << "Please review data entered. Would you like to save? (Y for yes)" << endl;
	cin >> save;
	bool good = true;
	if (save == 'Y' || 'y') {

		for (unsigned int i = 0; i < trackNum.length(); i++) {
			if (!isdigit(trackNum[i]) || trackNum.length() > 6) {
				cout << trackNum << " is not a 6 digit number!" << endl;
				good = false;
				break;
			}
		}
		for (unsigned int i = 0; i < numEggs.length(); i++) {
			if (!isdigit(numEggs[i])) {
				cout << numEggs << " is not a valid number!" << endl;
				good = false;
				break;
			}
		}
		for (unsigned int i = 0; i < isNursing.length(); i++) {
			if (!isdigit(isNursing[i])) {
				cout << isNursing << " is not a valid number!" << endl;
				good = false;
				break;
			}
		}
		// use stoi to convert strings to integers
		int trackI = stoi(trackNum);
		int eggsI = stoi(numEggs);
		int nurseI = stoi(isNursing);

		// if-else statements to check that user input is correct
		if (animalType != "Mammal" && animalType != "Oviparous") {
			cout << animalType << " is not Mammal or Oviparous." << endl;
			good = false;
		}
		if (animalType == "Mammal") {
			if (subType != "Bat" && subType != "Whale" && subType != "SeaLion") {
				cout << subType << " is not a Bat, Whale, or SeaLion." << endl;
				good = false;
			}
		}
		else {
			if (subType != "Crocodile" && subType != "Goose" && subType != "Pelican") {
				cout << subType << " is not a Crocodile, Goose, or Pelican." << endl;
				good = false;
			}
		}
		if (eggsI > 0 && nurseI > 0) {
			cout << "Information is invalid, please try again." << endl;
			good = false;
		}
		if (good) {
			animalName = animalName.substr(0, 15);

			// push back data to global vector WildLifeZoo
			if (animalType == "Mammal") {
				if (subType == "Bat") {
					WildLifeZoo.push_back(unique_ptr<Animal>(new Bat(animalName, trackI, nurseI)));
				}
				else if (subType == "Whale") {
					WildLifeZoo.push_back(unique_ptr<Animal>(new Whale(animalName, trackI, nurseI)));
				}
				else {
					WildLifeZoo.push_back(unique_ptr<Animal>(new SeaLion(animalName, trackI, nurseI)));
				}
			}
			else {
				if (subType == "Crocodile") {
					WildLifeZoo.push_back(unique_ptr<Animal>(new Crocodile(animalName, trackI, eggsI)));
				}
				else if (subType == "Goose") {
					WildLifeZoo.push_back(unique_ptr<Animal>(new Goose(animalName, trackI, eggsI)));
				}
				else {
					WildLifeZoo.push_back(unique_ptr<Animal>(new Pelican(animalName, trackI, eggsI)));
				}
			}
			cout << "Animal save complete!" << endl;
		}
		else {
			cout << "Information is invalid, please try again." << endl;
		}
		return;
   }
}

void RemoveAnimal() {			// allows user to remove animal from zoodata.txt file
	string trackS;
	cout << "Enter the Tracking Number of the Animal you want to remove." << endl;
	cin >> trackS;
	for (unsigned int i = 0; i < trackS.length(); i++) {
		if (!isdigit(trackS[i]) || trackS.length() > 6) {
			cout << trackS << " is not a valid input." << endl;
			return;
		}
	}
	int trackI = stoi(trackS);
	for(unsigned int i = 0; i < WildLifeZoo.size(); i++) {
		if(trackI == static_cast<Animal*>(WildLifeZoo[i].get())->TrackNum) {
			WildLifeZoo.erase(WildLifeZoo.begin() + i);
			cout << "Data has been deleted." << endl;
			return;
		}
	}
	cout << "Data does not exist, please try again.";
}

void LoadDataFromFile() {		// loads data from zoodata.txt file
	ifstream in ("zoodata.txt");
	string line;

	while (getline(in, line)) {
		if (line.size()>0) {
			int trackNum;
			string animalName;
			string animalType;
			string subType;
			int numEggs;
			int isNursing;

			trackNum = stoi(line.substr(0, 7));
			animalName = trim(line.substr(8, 15));
			animalType = trim(line.substr(25, 15));
			subType = trim(line.substr(42, 15));
			numEggs = stoi(line.substr(59, 1));
			isNursing = stoi(line.substr(61, 1));

			// use string npos to find elements in zoodata.txt file
			if (animalType.find("Oviparous") != string::npos) {

				if (subType.find("Crocodile") != string::npos) {
					WildLifeZoo.push_back(unique_ptr<Animal>(new Crocodile(animalName, trackNum, numEggs)));
				}
				if (subType.find("Goose") != string::npos) {
					WildLifeZoo.push_back(unique_ptr<Animal>(new Goose(animalName, trackNum, numEggs)));
				}
				if (subType.find("Pelican") != string::npos) {
					WildLifeZoo.push_back(unique_ptr<Animal>(new Pelican(animalName, trackNum, numEggs)));
				}
			}
			if (animalType.find("Mammal") != string::npos) {
				if (subType.find("Bat") != string::npos) {
					WildLifeZoo.push_back(unique_ptr<Animal>(new Bat(animalName, trackNum, isNursing)));
				}
				if (subType.find("Whale") != string::npos) {
					WildLifeZoo.push_back(unique_ptr<Animal>(new Whale(animalName, trackNum, isNursing)));
				}
				if (subType.find("SeaLion") != string::npos) {
					WildLifeZoo.push_back(unique_ptr<Animal>(new SeaLion(animalName, trackNum, isNursing)));
				}
			}
		}
	}
	cout << "Wildlife Zoo data has been loaded." << endl;
}
// functions to test for animal object type
string getType(int i) {
	if (dynamic_cast<Mammal*>(WildLifeZoo[i].get()) != 0) {
		return "Mammal";
	}
	else {
		return "Oviparous";
	}
}
string getSubType(int i) {
	if (getType(i) == "Mammal") {
		if (dynamic_cast<Bat*>(WildLifeZoo[i].get()) != 0) {
			return "Bat";
		}
		if (dynamic_cast<Whale*>(WildLifeZoo[i].get()) != 0) {
			return "Whale";
		}
		if (dynamic_cast<SeaLion*>(WildLifeZoo[i].get()) != 0) {
			return "SeaLion";
		}
	}
	else {
		if (dynamic_cast<Crocodile*>(WildLifeZoo[i].get()) != 0) {
			return "Crocodile";
		}
		if (dynamic_cast<Goose*>(WildLifeZoo[i].get()) != 0) {
			return "Goose";
		}
		if (dynamic_cast<Pelican*>(WildLifeZoo[i].get()) != 0) {
			return "Pelican";
		}
	}
	return "Unknown";
}
// code provided from ZooFileWriter.java
string padLeft(string str, int leng, string stringpadVal) {
for (int i = str.length(); i <= leng; i++)
str = stringpadVal + str;
return str;
}
string padRight(string str, int leng, string stringpadVal) {
for (int i = str.length(); i <= leng; i++)
str = str + stringpadVal;
return str;
}
// character limits required by wildlife zoo (6 characters, 15 characters, 15 characters, 15 characters, and two integers)
string dress(string trackNum, string animalName, string animalType, string subType, string numEggs, string isNursing){
	string out = "";
	out += padLeft(trackNum, 6, "0");
	out += " ";
	out += padRight(animalName, 15, " ");
	out += " ";
	out += padRight(animalType, 15, " ");
	out += " ";
	out += padRight(subType, 15, " ");
	out += " ";
	out += numEggs;
	out += " ";
	out += isNursing;

	return out;
}

void SaveDataToFile() {			// saves vector data to zoodata.txt
	ofstream out("zoodata.txt");
	for(unsigned int i = 0; i < WildLifeZoo.size(); i++){
		if(getType(i) == "Oviparous") {
			out << dress(to_string(static_cast<Animal*>(WildLifeZoo[i].get())->TrackNum),
				static_cast<Animal*>(WildLifeZoo[i].get())->Name, "Oviparous", getSubType(i), to_string(static_cast<Oviparous*>(WildLifeZoo[i].get())->NumberOfEggs), "0");
		}
		else if (getType(i) == "Mammal") {
			out << dress(to_string(static_cast<Animal*>(WildLifeZoo[i].get())->TrackNum),
				static_cast<Animal*>(WildLifeZoo[i].get())->Name, "Mammal", getSubType(i), "0", to_string(static_cast<Mammal*>(WildLifeZoo[i].get())->Nurse));
		}
		if (i < WildLifeZoo.size()-1) {
			out << endl;
		}
	}
	cout << "Data Saved" << endl;
}

void DisplayMenu()
{
	// display menu with options for the user to select
	cout << "******************************" << endl;
	cout << "*** Wildlife Zoo Interface ***" << endl;
	cout << "****** Select An Option ******" << endl;
	cout << "***                        ***" << endl;
	cout << "* 1: Load Animal Data        *" << endl;
	cout << "* 2: Generate Data           *" << endl;
	cout << "* 3: Display Animal Data     *" << endl;
	cout << "* 4: Add Record              *" << endl;
	cout << "* 5: Remove Record           *" << endl;
	cout << "* 6: Save Animal Data        *" << endl;
	cout << "* 7: Quit                    *" << endl;
	cout << "******************************" << endl;
}

void DisplayAnimalData() {		// displays data from zoodata.txt file
	cout << WildLifeZoo.size() << " Animals Found!" << endl;
	cout << dress(" Track#", "Name", "Type", "Sub-Type", "Eggs", "Nurse") << endl;
	for (unsigned int i = 0; i < WildLifeZoo.size(); i++) {
		if(getType(i) == "Oviparous") {
			cout << dress(to_string(static_cast<Animal*>(WildLifeZoo[i].get())->TrackNum), static_cast<Animal*>(WildLifeZoo[i].get())->Name, "Oviparous", getSubType(i), to_string(static_cast<Oviparous*>(WildLifeZoo[0].get())->NumberOfEggs), "0");
		}
		else if(getType(i) == "Mammal"){
           cout << dress(to_string(static_cast<Animal*>(WildLifeZoo[i].get())->TrackNum), static_cast<Animal*>(WildLifeZoo[i].get())->Name, "Mammal", getSubType(i), "0", to_string(static_cast<Mammal*>(WildLifeZoo[0].get())->Nurse));
		}
		cout << endl;
	}
}

int main() {
    int option = 0;
    bool cont = false;		// true/false statement
    do {
    	cont = true;

    	DisplayMenu();
    	cin >> option; 		// program receives user input

    	// switch/case statements to allow variables to be tested
    	switch(option){
    	case 1:
    		LoadDataFromFile();		// loads data from zoodata.txt file
    		break;
    	case 2:
    		GenerateData();			// default java data
    		break;
    	case 3:
    		DisplayAnimalData();	// displays data from zoodata.txt file
    		break;
    	case 4:
    		AddAnimal();			// allows user to add data for a new animal
    		break;
    	case 5:
    		RemoveAnimal();			// allows user to remove animal from zoodata.txt file
    		break;
    	case 6:
    		SaveDataToFile();		// saves vector data to zoodata.txt
    		break;
    	case 7:
    	default:
    		cont = false;
    		break;
    	}
    }
    while(cont);

    return 1;
}
