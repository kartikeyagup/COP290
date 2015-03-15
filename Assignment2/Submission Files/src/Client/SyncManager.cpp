#include "SyncManager.h"

bool SyncListContains(SyncList cons, std::string tofind)
{
	bool found=false;
	for (int i=0; (i<cons.ListOfFiles.size()) && !(found) ; i++)
	{
		if (cons.ListOfFiles[i]==tofind)
		{
			found=true;
		}
	}
	return found;
}


SyncManager::SyncManager()
{
	Username="";
	SyncList x;
	x.ListOfFiles = std::vector<std::string> ();
	FilesToSync =x;
	MainFiles=FileChanges();
	ReceivingFiles=FileReceiving();
	GivingFiles = FileSharing();
}

SyncManager::SyncManager(std::string n1)
{
	Username=n1;
	SyncList x;
	x.ListOfFiles = std::vector<std::string> ();
	FilesToSync =x;
	MainFiles=FileChanges("/home/kartikeya/Desktop/DeadDrop/"+n1, "/home/faran/Desktop/DeadDropServer/"+n1);
	ReceivingFiles=FileReceiving();
	GivingFiles = FileSharing();	
}

FileChanges SyncManager::GetMainFiles()
{
	return MainFiles;
}

FileReceiving SyncManager::GetReceivingFiles()
{
	return ReceivingFiles;
}

FileSharing SyncManager::GetGivingFiles()
{
	return GivingFiles;
}

SyncList SyncManager::GetFilesToSync()
{
	return FilesToSync;
}

std::string SyncManager::GetUsername()
{
	return Username;
}

FileHistory SyncManager::GetCurrentClientHistory()
{
	// TODO:
	// Open folder
	// Files discover kare le ao
}

void SyncManager::SetUsername(std::string n)
{
	Username=n;
}

void SyncManager::SetMainFiles(FileChanges Newf)
{
	MainFiles =Newf;
}

void SyncManager::SetReceivingFiles(FileReceiving Recf)
{
	ReceivingFiles = Recf;
}

void SyncManager::SetGivingFiles(FileSharing files)
{
	GivingFiles = files;
}

void SyncManager::SetFilesToSync(SyncList slist)
{
	FilesToSync = slist;
}

void SyncManager::AddFileToSync(std::string nfile)
{
	std::vector<std::string>::iterator position = std::find(FilesToSync.ListOfFiles.begin(), FilesToSync.ListOfFiles.end(), nfile);
	
	if (position == FilesToSync.ListOfFiles.end())
	{	
		FilesToSync.ListOfFiles.push_back(nfile);
	}
}

void SyncManager::RemoveFileFromSync(std::string nfile)
{
	std::vector<std::string>::iterator position = std::find(FilesToSync.ListOfFiles.begin(), FilesToSync.ListOfFiles.end(), nfile);
	if (position != FilesToSync.ListOfFiles.end()) // == vector.end() means the element was not found
	{
 	   FilesToSync.ListOfFiles.erase(position);
	}
}

void SyncManager::RefreshClientFolder()
{
	MainFiles.RefreshClient();
}

void SyncManager::LoadSyncList(std::string location)
{
	std::string line1;
	std::ifstream myfile (location);
	FilesToSync.ListOfFiles = std::vector<std::string> ();
	if (myfile.is_open())
	{
		while ( getline (myfile,line1) )
		{	
			FilesToSync.ListOfFiles.push_back(line1);
		}
		myfile.close();
	}
}

void SyncManager::LoadReceiving(std::string location)
{
	ReceivingFiles.LoadSharingFromDisk(location);
}

void SyncManager::LoadGiving(std::string location)
{
	GivingFiles.LoadGivingFromDisk(location);
}

void SyncManager::LoadClServerMap(std::string location)
{
	MainFiles.LoadFileLinkingClient(location);
}

void SyncManager::LoadSeClientMap(std::string location)
{
	MainFiles.LoadFileLinkingServer(location);
}

void SyncManager::LoadClHistory(std::string location)
{
	MainFiles.LoadClientHistory(location);
}

void SyncManager::LoadSeHistory(std::string location)
{
	// std::cout <<"Loading se history\n";
	MainFiles.LoadServerHistory(location);
}

void SyncManager::StoreSyncList(std::string location)
{
#ifdef DEBUG
	std::cout <<"Store sync list location: " <<location <<"\n";
#endif
	std::string data="";
	for (int i=0; i<FilesToSync.ListOfFiles.size() ; i++) 
	{
		data += FilesToSync.ListOfFiles[i] + "\n";
	}
	data=data.substr(0,data.size() - 1);
	std::ofstream out(location);
	out << data;
	out.close();
#ifdef DEBUG
	std::cout <<"File stored \n";
#endif
#ifdef DEBUG
	std::cout << data <<"\n";
#endif
}

void SyncManager::StoreReceiving(std::string location)
{
	ReceivingFiles.StoreSharingToDisk(location);
}

void SyncManager::StoreGiving(std::string location)
{
	GivingFiles.StoreGivingToDisk(location);
}

void SyncManager::StoreClServerMap(std::string location)
{
	MainFiles.StoreFileLinkingClient(location);
}

void SyncManager::StoreSeClientMap(std::string location)
{
	MainFiles.StoreFileLinkingServer(location);
}

void SyncManager::StoreClHistory(std::string location)
{
	MainFiles.StoreClientHistory(location);
}

void SyncManager::StoreSeHistory(std::string location)
{
	MainFiles.StoreServerHistory(location);
}

void SyncManager::LoadFromDiskDB(std::string location)
{
	LoadSyncList(location+ "/" + Username+"/.data/synclist.txt");
	LoadReceiving(location+ "/" + Username+"/.data/receiving.txt");
	LoadGiving(location+ "/" + Username+"/.data/giving.txt");
	LoadClServerMap(location+ "/" + Username+"/.data/clserver.txt");
	LoadSeClientMap(location+ "/" + Username+"/.data/seclient.txt");
	LoadClHistory(location+ "/" + Username+"/.data/clhistory.txt");
	LoadSeHistory(location+ "/" + Username + "/.data/sehistory.txt");	
}

void SyncManager::StoreToDiskDB(std::string location)
{
	StoreSyncList( location + "/" + Username + "/.data/synclist.txt");
	StoreReceiving( location + "/" + Username + "/.data/receiving.txt");
	StoreGiving( location +"/" + Username + "/.data/giving.txt" );
	StoreClServerMap( location + "/" + Username + "/.data/clserver.txt");
	StoreSeClientMap( location + "/" + Username + "/.data/seclient.txt");
	StoreClHistory( location + "/" + Username + "/.data/clhistory.txt");
	StoreSeHistory( location + "/" + Username + "/.data/sehistory.txt");		
}

std::string SyncManager::GetClientMappingForFile(std::string serverfilename)
{
	// Gives the server file name for input
	return MainFiles.GetCLMapping(serverfilename);
}

std::string SyncManager::GetServerMappingForFile(std::string serverfilename)
{
	// Gives the client file name for input
	return MainFiles.GetSEMapping(serverfilename);
}


void SyncManager::RemoveFromServerBase(std::string serverfilename)
{
	// Remove from sehistory
	// Remove from clhistory
	MainFiles.RemoveFromServer(serverfilename);
	std::string clientfilename = GetClientMappingForFile(serverfilename);
	MainFiles.RemoveFromClient(clientfilename);
}

void SyncManager::RemoveFromClientBase(std::string clientfilename)
{
	// Remove from sehistory
	// Remove from clhistory
	MainFiles.RemoveFromClient(clientfilename);
	std::string serverfilename = GetServerMappingForFile(clientfilename);
	MainFiles.RemoveFromServer(serverfilename);
}


std::vector<Instruction> SyncManager::GetSyncingInstructions()
{
	// Get current client files in temp storage
	// Reload client db files
	// See for files in sync list which are no longer on disc
	// See for files in sync list which are in disc and changed
	// See for files in sync list which are not changed on disc
	// See for files not in sync list but are in temp storage

	std::vector<Instruction> answer;
	FileHistory CLH = MainFiles.GetClientFiles();
	FileHistory SEH = MainFiles.GetServerFiles();
	FileHistory SEHcopy = MainFiles.GetServerFiles();
	UserFiles USF = MainFiles.GetFileLinking();

	FileHistory PresentFiles= GetFilesOnDisc(CLH.GetFolder());
	// CLH = PresentFiles;

	int numclient = CLH.GetNumberOfFiles();
	int numcpresent = PresentFiles.GetNumberOfFiles();
	int numserver = SEH.GetNumberOfFiles();

	bool fileClient [numclient];
	bool filePresent [numcpresent];
	bool fileServer [numserver];

	int ClientPresent [numclient];

	for (int i=0; i<numclient; i++)
	{
		fileClient[i]=false;
		ClientPresent[i]= -1;
	}
	
	for (int i=0; i<numserver; i++)
	{
		fileServer[i]=false;
	}

	for (int i=0; i<numcpresent; i++)
	{
		filePresent[i]=false;
	}

	for (int i=0; i< numclient ; i++)
	{
		std::string ClData = CLH.GetNthName(i);
		for (int j=0; (j<numcpresent) && !(fileClient[i]); j++)
		{
			if (ClData == PresentFiles.GetNthName(j))
			{
				filePresent[j]=true;
				fileClient[i]=true;
				ClientPresent[i]=j;
			}
		}
	}

	std::vector<std::string> todiscard;

	std::vector<std::string> filesonserverfound;
	std::vector<std::string> filesonserverremove;

#ifdef DEBUG
	std::cout <<"Mapped prev and current files\n";
#endif
	for (int i=0; i < numclient ; i++)
	{
		if (fileClient[i])
		{
			// File was there on last sync and now as well
			// If change occured then do work
			int positionlocal=ClientPresent[i];
			// if (SyncListContains(FilesToSync, CLH.GetNthName(i)))
			if (true)
			{
				std::string servername = USF.GetClientFileName(CLH.GetNthName(i)); 
				bool d1=false;
				for (int k=0; (k<numserver); k++)
				{
					if (servername == SEH.GetNthInfo(k).first)
					{
						fileServer[k]=true;
						filesonserverfound.push_back(servername);
						d1=true;
						if (PresentFiles.GetNthTime(positionlocal) > CLH.GetNthTime(i))
						{
							Instruction a;
							a.modification=3;
							a.data1= CLH.GetNthName(i);
							a.data2= SEH.GetNthName(k);
							answer.push_back(a);
						}
						else
						{
							if (SEH.GetNthTime(k)>PresentFiles.GetNthTime(positionlocal))
							{
								Instruction a;
								a.modification = 2;
								a.data1=PresentFiles.GetNthName(positionlocal);
								a.data2=SEH.GetNthName(k);
								answer.push_back(a);
								CLH.SetNthTime( i , SEH.GetNthTime(k));
							}
						}
					}
				}
				if (!d1)
				{
					Instruction a;
					a.modification = 5;
					a.data1=CLH.GetNthName(i);
					answer.push_back(a);
					CLH.RemoveFile(a.data1);
					// Delete file on local
					// Remove from mappings and history
				}
			}
			else
			{
				// Might cause issue with shared files
				Instruction a;
				a.modification = 5;
				a.data1 = CLH.GetNthName(i);
				answer.push_back(a);
				CLH.RemoveFile(CLH.GetNthName(i));		
			}
		}
		else
		{
			// File was there at last sync but not anymore
			// Delete this file from cloud
			// Remove from db
			// Remove file from synclist
			Instruction a;
			a.modification = 6;
			a.data2 = CLH.GetNthName(i);
			std::string servername = USF.GetClientFileName(CLH.GetNthName(i)); 
			// RemoveFileFromSync (CLH.GetNthName(i));
			RemoveFromClientBase(CLH.GetNthName(i));
			CLH.RemoveFile(CLH.GetNthName(i));
			SEHcopy.RemoveFile(servername);
			filesonserverremove.push_back(servername);
			a.data1 = servername;
			answer.push_back(a);	
		}
	}

#ifdef DEBUG
	std::cout <<"Done with files which were there before and now\n";
#endif

	for (int i=0; i < numcpresent ; i++)
	{
		if (!(filePresent[i]))
		{
			// New file has appeared on client
			// Send this to the server
			Instruction a;
			//AddFileToSync(PresentFiles.GetNthName(i));
			a.modification=3;
			a.data1= PresentFiles.GetNthName(i);
			std::string sname=a.data1;
			int lenpath= CLH.GetFolder().size();
			sname=sname.substr(lenpath);
			a.data2=SEH.GetFolder()+sname;
			// USF.AddNew(a.data1,a.data2);
			answer.push_back(a);
		}
	}

#ifdef DEBUG
	std::cout <<"Done with new files\n";
#endif
	
	numserver = SEHcopy.GetNumberOfFiles();

	for (int i=0; i < numserver ; i++)
	{
		bool f1=false;
		for (int j=0; j <filesonserverfound.size() ; j++)
		{
			if (filesonserverfound[j]==SEHcopy.GetNthName(i))
			{
				f1 =true;
			}
		}
		if (!f1)
		{
#ifdef DEBUG
			std::cout << i <<"\t" <<fileServer[i] <<"\n";
#endif
			// New file on server
			// Get it to client
			Instruction a;
			a.modification = 2;
			a.data2=SEHcopy.GetNthName(i);
#ifdef DEBUG
			std::cout <<a.data2 <<"\n";
#endif

			std::string mainpath(getenv("HOME")); 
			std::string foldername=mainpath + "/Desktop/DeadDrop/" + GetUsername()  + "/";
			std::string serverfoldername="/home/faran/Desktop/DeadDropServer/" + GetUsername() +"/";

			std::string clpath1= foldername + a.data2.substr(serverfoldername.size());
#ifdef DEBUG
			std::cout <<"NEW CL PATH\n" << clpath1 <<"\n";
#endif

			a.data1=clpath1;
#ifdef DEBUG
			std::cout <<a.data1<<"\n";
#endif
			answer.push_back(a);
			CLH.AddFileToHistory(clpath1, SEHcopy.GetNthTime(i));
			USF.AddNew(a.data1, a.data2);
			// AddFileToLinking(a.data1,a.data2);
		}
	}

#ifdef DEBUG
	std::cout << "Done with new files on server\n";
#endif

#ifdef DEBUG
	std::cout <<"Done with this majorly\n";
#endif
	MainFiles.SetFileLinking(USF);
	MainFiles.SetClientFiles(CLH);
	MainFiles.SetServerFiles(SEHcopy);
	std::string mainpath(getenv("HOME")); 
	std::string foldername=mainpath + "/Desktop/DeadDrop/" + Username  + "/";
	StoreToDiskDB(mainpath+"/Desktop/DeadDrop/");
	return answer;
}


void SyncManager::SaveInstructionVector(std::vector<Instruction> InstructionVector, std::string location)
{
	std::string data="";
	for (int i=0; i<InstructionVector.size() ; i++)
	{
		data += InstructionVector[i].data1 + "\n" + InstructionVector[i].data2 +"\n" + std::to_string(InstructionVector[i].modification)+"\n";
	}
	data=data.substr(0,data.size()-1);
	std::ofstream out(location);
	out << data;
	out.close();
}

std::vector<Instruction> SyncManager::LoadInstructionVector(std::string location)
{
	std::string line1,line2,line3;
	std::ifstream myfile (location);
	std::vector<Instruction> answer;
	if (myfile.is_open())
	{
		while ( getline (myfile,line1) )
		{	
    		getline(myfile,line2);
    		getline(myfile,line3);
  			Instruction a;
  			a.data1= line1;
  			a.data2= line2;
  			a.modification= std::stoi(line3);
    		answer.push_back(a);
    	}
    	myfile.close();
  	}	
  	return answer;
}

void SyncManager::AddFileToLocalFiles(std::string locpath, std::string gpath)
{
	MainFiles.AddFilesToFileChanges(locpath,gpath);
}

void SyncManager::UpdateSyncTimes()
{
	MainFiles.SetTimeChanges(std::time(0));
}

void SyncManager::AddFileToLinking(std::string cl, std::string se)
{
	UserFiles USF = MainFiles.GetFileLinking();
	USF.AddNew(cl,se);
	MainFiles.SetFileLinking(USF);
}

Instruction DoLogin(std::string us,std::string pwd)
{
	Instruction p;
	p.modification=7;
	p.data1=us;
	p.data2=pwd;
	return p;
}

Instruction NewUser(std::string us, std::string pwd)
{
	Instruction p;
	p.modification=8;
	p.data1=us;
	p.data2=pwd;
	return p;
}

Instruction Exit()
{
	Instruction p;
	p.modification=10;
	return p;
}

Instruction UserExists(std::string usname)
{
	Instruction p;
	p.modification=9;
	p.data1=usname;
	p.data2="";
	return p;
}

Instruction GetServerFiles(std::string clpath,std::string sepath)
{
	Instruction p;
	p.modification=11;
	p.data1=clpath;
	p.data2=sepath;
	return p;
}

Instruction TransferServerToClient(std::string clpath,std::string sepath)
{
	Instruction p;
	p.modification=2;
	p.data1=clpath;
	p.data2=sepath;
	return p;
}

Instruction TransferClientToServer(std::string clpath, std::string sepath)
{
	Instruction p;
	p.modification=1;
	p.data1=clpath;
	p.data2=sepath;
	return p;
}

Instruction DeleteFileOnServer(std::string location)
{
	Instruction p;
	p.modification=6;
	p.data1=location;
	p.data2=location;
	return p;	
}

Instruction DeleteFileOnClient(std::string location)
{
	Instruction p;
	p.modification=5;
	p.data1=location;
	p.data2=location;
	return p;	
}

Instruction RefreshServerDB(std::string location)
{
	Instruction p;
	p.modification=12;
	p.data1=location;
	p.data2=location;
	return p;	
}

Instruction DoNothing()
{
	Instruction p;
	p.modification=0;
	p.data1="";
	p.data2="";
	return p;
}

Instruction DoNormalSending(std::string s1, std::string s2)
{
	Instruction p;
	p.modification=14;
	p.data1=s1;
	p.data2=s2;
	return p;	
}

Instruction ChangePasswordIns(std::string usname, std::string opwd, std::string npwd)
{
	Instruction p;
	p.modification=16;
	p.data1=usname;
	p.data2=opwd;
	p.data3=npwd;
	return p;
}
