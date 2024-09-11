#include "mainheader.h" //header file for the functions
#include <stdio.h>
#include <stdexcept>

int main(void)
{
	std::string command; //command to start the program
	std::cout << "- Welcome to the Process Priority Changer" << std::endl;

	std::cout << "- Please Enter Process ID...." << std::endl;
	std::cin >> ProcessID; //get the process id from the user
	if (Get_Process_Handle(PROCESS_ALL_ACCESS, ProcessID)) { //returns the hProcess handle and the processid
		std::cout << "Error Occured" << std::endl;
		goto Cleanup;
	}
	GetProcessImageFileName(hProcess, ProcessName, MAX_PATH); //returns the process name
	std::wcout << L"- Process Path is " << ProcessName << std::endl;  //print the process id and the process name


	while (command != "stop") {
		std::cout << "\n- What do you want to do:\n1- Set Process Priority Class\n2- Set Process Affinity\n3- Set Threads Priority\n4- Set Threads Affinity\n==> " << std::endl;

		try {
			std::cin >> command;
			if (command == "1")
			{
				if (Set_Process_Priority(hProcess, Process_Priority_Class)) { //sets the priority of the process to the desiered class
					std::cout << "Error Occured" << std::endl;
					goto Cleanup;
				}
			}
			else if (command == "2")
			{
				if (Set_Process_Affinity(hProcess, Mask)) { // set affinity mask (on how many cores do you want the procss to run)
					std::cout << "Error Occured" << std::endl;
					goto Cleanup;
				}
			}
			else if (command == "3")
			{
				Get_Process_Snapshot(ProcessID); //get the thread snapshot
				if (Set_Threads_Priority(hThreadSnap, Thread_Priority_Class)) { //sets the priority of the thread to the desiered class
					std::cout << "Error Occured" << std::endl;
					goto Cleanup;
				}
			}
			else if (command == "4")
			{
				if(Set_Threads_Affinity(hThreadSnap, Mask)){ // set affinity mask (on how many cores do you want the procss to run)
					std::cout << "Error Occured" << std::endl;
					goto Cleanup;
				}
			}
			else throw std::runtime_error("Invalid Command");
		}
		catch ( const std::exception e) {
			std::cout<< "Exception " << e.what() << std::endl;
			goto Cleanup;
		}
	}

	//Get_Process_Handle(PROCESS_ALL_ACCESS, ProcessID); //returns the hProcess handle and the processid
	//GetProcessImageFileName(hProcess, ProcessName, MAX_PATH); //returns the process name
	//std::wcout << L"- Process Path is " << ProcessName << std::endl;  //print the process id and the process name

	//Set_Process_Priority(hProcess, Process_Priority_Class); //sets the priority of the process to the desiered class

	//Set_Process_Affinity(hProcess, Mask); // set affinity mask (on how many cores do you want the procss to run)

	//Get_Process_Snapshot(ProcessID); //get the thread snapshot
	// 
	//Set_Threads_Priority(hThreadSnap, Thread_Priority_Class); //sets the priority of the thread to the desiered class
Cleanup:
	// Clean up
	CloseHandle(hProcess);
	CloseHandle(hThreadSnap);
	CloseHandle(hThread);
	std::cout << "Goodbye" << std::endl;
	return 0;
}