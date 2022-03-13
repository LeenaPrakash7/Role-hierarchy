#include<bits/stdc++.h>
using namespace std;

class company 
{
    public:
        string rolename;
        string parent;
        static int count;
        int rid;
        list<string> userlist;
        list<string> child; 
        void setId()
        {
            count++;
       
        }
        company()
        {
            rid=-1;
        }
        company(string newrole) 
        {
            setId();
            rolename=newrole;
            this->parent="\0";
        }
        company(string newrole,string parent)
        {
            setId();
            rolename=newrole;
            this->parent=parent;
        }
        
};
class tree 
{
    public:
        company rootrole; 
        list<company> hierarchy; 
        list<pair<string,string>> alluser; 
        int sizeofhierarchy=0;
        void display()  //function to display the roles   
        {
            queue<string> que;
            que.push(rootrole.rolename);
            while(!que.empty())
            {
                string currrolename=que.front();
                company *currrole=findrolebyname(currrolename);
                for(string childroles : currrole->child)
                {
                    que.push(childroles);
                }
                if(rootrole.rolename!=currrolename)
                cout<<",";
                cout<<currrolename;
                que.pop();
            }
            cout<<endl;
        }
        void setroot(company c) 
        {
            rootrole=c;
            hierarchy.push_back(c);
        }
        company *findrolebyname(string Name)
        {
            for(company &parent : hierarchy)
            {
                if(parent.rolename==Name)
                return &parent;
            }
            cout<<"Role not found\n";
            company norole;
            return NULL;
        }
        void addrole(company c) //function to add a new role 
        {
            sizeofhierarchy++;
            hierarchy.push_back(c);
            company *parent=findrolebyname(c.parent);
            if(parent!=NULL)
            parent->child.push_back(c.rolename);    
        }
        void deleterole(string removerole,string parent) //function to delete a role - case 3 
        {
            sizeofhierarchy--;
            company *todelete=findrolebyname(removerole);
            company *parentOfdelete=findrolebyname(todelete->parent);
            company *newparent=findrolebyname(parent);
            parentOfdelete->child.remove(removerole);
            newparent->child.merge(todelete->child);
            newparent->userlist.merge(todelete->userlist);
            
            list<company> temp;
            for(company &c : hierarchy)
            {
                if(c.rolename!=removerole)
                {
                    temp.push_back(c);
                }
            }
            hierarchy=temp;
            for(auto &userpair : alluser)
            {
                if(userpair.second==removerole)
                    userpair.second=parent;
            }
        }
        list<string> mergeuser(company *currrole)  
        {
            list<string> subs,currroleuser;
            currroleuser=currrole->userlist;
            for(auto subrole : currrole->child)
            {
                subs.merge(mergeuser(findrolebyname(subrole)));
            }
            subs.merge(currroleuser);
            return subs;
        }
        list<string> dfsonuser(string Name) //depth first search - display all users fn 
        {
            list<string> subs,currroleuser;
            company *currrole=findrolebyname(Name);
            currroleuser=currrole->userlist;
            for(auto subrole : currrole->child)
            {
                subs.merge(dfsonuser(subrole));
            }
            for(auto &curruser : currroleuser)
            {
                cout<<curruser<<"->";
                for(auto strIt=subs.begin();strIt!=subs.end();strIt++)
                {
                    if(strIt!=subs.begin())
                    cout<<",";
                    cout<<(*strIt);
                }
                cout<<endl;
            }
            subs.merge(currroleuser);
            return subs;
        }

        
        int maxheight(string Name) // height of hierarchy fn 
        {
            company *currrole=findrolebyname(Name);
            int maxofchild=0;
            if(currrole->child.empty())
            return 0;
            else
            {   
                for(string &childrole : currrole->child)
                    maxofchild=max(maxofchild,maxheight(childrole));
                return 1+maxofchild;
            }
        }
        void lcaofusers(company *currrole,string user1,string user2) //lowest common ancestor - case 10
        {
           list<string> subs;
           bool found1=false,found2=false;
           if(currrole->userlist.empty())
           {
               for(auto &child : currrole->child)
               {
                    lcaofusers(findrolebyname(child),user1,user2);
               }
           }
           else
           {
               subs=mergeuser(currrole);
               for(auto &str : subs)
                {
                if(str==user1)
                found1=true; 
                if(str==user2)
                found2=true;
                }
                if(found1 && found2)
                {
                    cout<<"Top most common bosses are:"<<endl;
                    for(auto str=currrole->userlist.begin();str!=currrole->userlist.end();str++)
                    {
                        if(str!=currrole->userlist.begin())
                        cout<<",";
                        cout<<(*str);
                    }
                    cout<<endl;
                }
           }
        }
};
int company::count=0;
int main()
{
    int choice,countuser;
    tree t;
    company c;
    company *roleofuser;
    string newrole,parent,removerole,Name,user1,user2;
    cout<<"Enter the top most role name :";
    getline(cin,newrole);
    c=company(newrole);
    t.setroot(c);
    cout<<"Company Owner :"<<newrole<<endl;
    do
    {
        cout<<"Operations :"<<endl;
        cout<<"\tPress 1 to add a sub title\n";
        cout<<"\tPress 2 to display titles\n";
        cout<<"\tPress 3 to delete a title\n";
        cout<<"\tPress 4 to add an employee name\n";
        cout<<"\tPress 5 to display employee names\n";
        cout<<"\tPress 6 to display employee and sub employee\n";
        cout<<"\tPress 7 to delete an employee\n";
        cout<<"\tPress 8 to find the number of employees from top\n";
        cout<<"\tPress 9 to find the height of role hierarchy\n";
        cout<<"\tPress 10 to find the common boss of all employee\n";
        cout<<"\t If you wish to exit then press 0\n";
        cin>>choice;
        countuser=0;

        if(choice>0 && choice<=10)
        cout<<"Operation to be performed : "<<choice<<endl;
        switch(choice)
        {
            case 0 :break;
            case 1 :cout<<"Enter the title of the sub role :";
                    cin.ignore();
                    getline(cin,newrole);
                    cout<<"Enter the reporting role title  :";
                    getline(cin,parent);
                    c=company(newrole,parent);
                    t.addrole(c);
                    break;
            case 2 :t.display();
                    break;
            case 3 :cin.ignore();
                    cout<<"Enter the role to be deleted :";
                    getline(cin,removerole);
                    cout<<"Enter the role to be transferred :";
                    getline(cin,parent);
                    t.deleterole(removerole,parent);
                    
                    break;
            case 4 :cin.ignore();
                    cout<<"Enter employee name :";
                    getline(cin,Name);
                    cout<<"Enter designation :";
                    getline(cin,newrole);
                    roleofuser=t.findrolebyname(newrole);
                    if(roleofuser!=NULL)
                    {roleofuser->userlist.push_back(Name);
                    t.alluser.push_back(make_pair(Name,newrole)); 
                    }
                    break;
            case 5 :for(auto &userpair : t.alluser)
                    {
                        cout<<userpair.first<<"->"<<userpair.second<<endl;
                    }
                    break;
            case 6 :t.dfsonuser(t.rootrole.rolename);
                    break;
            case 7 :cin.ignore();
                    cout<<"Enter the employee name to delete :";
                    getline(cin,Name);
                    newrole="\0";
                    for(auto userpair=t.alluser.begin();userpair!=t.alluser.end();userpair++)
                    {
                        if(userpair->first==Name)
                        {
                            newrole=userpair->second;
                            t.alluser.erase(userpair);
                            break;
                        }
                    }
                    if(newrole!="\0")
                    {
                        roleofuser=t.findrolebyname(newrole);
                    if(roleofuser!=NULL)
                        roleofuser->userlist.remove(Name);
                    }
                    else
                    cout<<"Not found!\n";
                    break;
            case 8 :cin.ignore();
                    cout<<"Enter employee name :";
                    getline(cin,Name);
                    newrole="\0";
                    for(auto userpair=t.alluser.begin();userpair!=t.alluser.end();userpair++)
                    {
                        if(userpair->first==Name)
                        {
                            newrole=userpair->second;
                            break;
                        }
                    }
                    if(newrole!="\0")
                    {
                        roleofuser=t.findrolebyname(newrole);
                    }
                    else
                    {
                        cout<<"Employee doesn't exists !!!!!!!"<<endl;
                        break;
                    }
                    
                    while(roleofuser->parent!="\0")
                    {
                        roleofuser=t.findrolebyname(roleofuser->parent);
                        if(roleofuser==NULL)
                        break;
                        countuser+=roleofuser->userlist.size();
                    }
                    cout<<"Number of employees from top :"<<countuser<<endl;
                    break;
            case 9 :cout<<"Height of hierarchy :"<<t.maxheight(t.rootrole.rolename)+1<<endl;
                    break;
            case 10 :cin.ignore();
                    cout<<"Enter first employee  :";
                    getline(cin,user1);
                    cout<<"Enter second employee :";
                    getline(cin,user2);
                    t.lcaofusers(t.findrolebyname(t.rootrole.rolename),user1,user2);
                    break;
            default :cout<<"Invalid !!!!!!!\n";
        }
    } while (choice!=0);

    return 0;
}
