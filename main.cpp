#include <bits/stdc++.h>
#include <windows.h>
using namespace std;

HANDLE console_color = GetStdHandle(STD_OUTPUT_HANDLE);

fstream schemafile;

void fetchSchema(string tableName, vector<string> &schema);

bool checkTable(string tableName)
{
    schemafile.open("Schema.txt", ios::in);
    string line;
    bool flag = false;
    while (getline(schemafile, line))
    {
        if (line.substr(0, line.find('#')) == tableName)
        {
            flag = true;
            break;
        }
    }
    schemafile.close();
    return flag;
}

void createTable(vector<string> cmd)
{

    string table_name = cmd[2];

    if (checkTable(table_name))
    {
        SetConsoleTextAttribute(console_color, 12);
        cout << "Error: Table Already Exists" << endl;
        SetConsoleTextAttribute(console_color, 15);
        return;
    }

    for(int i=0; i<cmd.size(); i++) 
    {
        if(i+1 > 5 && ((i+1) % 3) == 0) 
        {
            if(!(cmd[i] == "STR" || cmd[i] == "INT")) 
            {
                SetConsoleTextAttribute(console_color, 12);
                cout<<"Error: Syntax Error: Missing data type "<<endl;
                SetConsoleTextAttribute(console_color, 15);
                return;
            }
        }
    }

    schemafile.open("Schema.txt", ios::app);
    schemafile << table_name;

    int start = -1, end = -1;
    for (int i = 3; i < cmd.size(); i++)
    {
        if (cmd[i] == "(")
        {
            start = i;
        }
        if (cmd[i] == ")")
        {
            end = i;
        }
    }

    if (start == -1 || end == -1)
    {
        SetConsoleTextAttribute(console_color, 12);
        cout << "Error: Syntax Error" << endl;
        SetConsoleTextAttribute(console_color, 15);
        return;
    }

    bool success = true;

    while (start < end - 1)
    {
        start++;
        if (cmd[start] != ",")
        {
            if((start - 5) % 3 == 0) 
            {
                if(!(cmd[start] == "INT" || cmd[start] == "STR")) 
                {
                    SetConsoleTextAttribute(console_color, 12);
                    cout<<"Error: Invalid data type: "<<cmd[start]<<endl;
                    cout<<"Table not created"<<endl;
                    SetConsoleTextAttribute(console_color, 15);
                    success = false;
                    break;
                }
            }
            schemafile << "#" << cmd[start];
        }
    }
    schemafile << endl;
    
    if(success) 
    {
        SetConsoleTextAttribute(console_color, 10);
        cout << "Table created successfully" << endl;
        SetConsoleTextAttribute(console_color, 15);
        schemafile.close();
    }
    else 
    {
        fstream temp;
        temp.open("temp.txt", ios::out);

        string line;
        while (getline(schemafile, line))
        {
            string l1 = line.substr(0, line.find('#'));
            if (table_name != l1)
            {
                temp << line << endl;
            }
        }

        schemafile.close();
        temp.close();

        remove("Schema.txt");
        rename("temp.txt", "Schema.txt");

        string f = table_name + ".txt";
        char fileName[f.length()];

        for (int i = 0; i < f.length(); i++)
        {
            fileName[i] = f[i];
        }
        fileName[f.length()] = '\0';

        // cout<<fileName<<endl;
        remove(fileName);
    }
}

void dropTable(vector<string> cmd)
{
    string table_name = cmd[2];
    if (!checkTable(table_name))
    {
        SetConsoleTextAttribute(console_color, 12);
        cout << "Error: Table Not Exists" << endl;
        return;
        SetConsoleTextAttribute(console_color, 15);
    }

    schemafile.open("Schema.txt", ios::in);

    fstream temp;
    temp.open("temp.txt", ios::out);

    string line;

    while (getline(schemafile, line))
    {
        string l1 = line.substr(0, line.find('#'));
        if (table_name != l1)
        {
            temp << line << endl;
        }
    }

    schemafile.close();
    temp.close();

    remove("Schema.txt");
    rename("temp.txt", "Schema.txt");

    string f = table_name + ".txt";
    char fileName[f.length()];

    for (int i = 0; i < f.length(); i++)
    {
        fileName[i] = f[i];
    }
    fileName[f.length()] = '\0';

    // cout<<fileName<<endl;
    remove(fileName);

    SetConsoleTextAttribute(console_color, 10);
    cout << "Table dropped successfully" << endl;
    SetConsoleTextAttribute(console_color, 15);
}

void describe(vector<string> cmd)
{
    schemafile.open("Schema.txt", ios::in);

    if (cmd.size() > 1)
    {
        string table_name = cmd[1];
        string line;
        bool found = false;

        while (getline(schemafile, line))
        {
            string l1 = line.substr(0, line.find('#'));

            if (table_name == l1)
            {
                string l2 = line.substr(line.find('#') + 1, line.size());
                int cnt = 1;

                for (auto i : l2)
                {
                    if (i == '#')
                    {
                        if (cnt % 2 != 0)
                        {
                            cout << "\t--\t";
                        }
                        else
                        {
                            cout << endl;
                        }
                        cnt++;
                    }
                    else
                    {
                        cout << i;
                    }
                }
                found = true;
                break;
            } 
        }
        if(!found) 
        {
            SetConsoleTextAttribute(console_color, 12);
            cout<<"Error: Table does not exist";
            SetConsoleTextAttribute(console_color, 15);
        }
        cout<<endl;
    }
    else
    {
        SetConsoleTextAttribute(console_color, 12);
        cout << "Error: Please mention table name in describe command" << endl;
        SetConsoleTextAttribute(console_color, 15);
    }
    schemafile.close();
    cout<<endl;
}

bool is_number(const std::string &s)
{
    std::string::const_iterator it = s.begin();
    while (it != s.end() && std::isdigit(*it))
        ++it;
    return !s.empty() && it == s.end();
}

void getDatatype(string tableName, vector<string> &data)
{
    vector<string> schema;
    fetchSchema(tableName, schema);
    for (int i = 2; i < schema.size(); i += 2)
    {
        data.push_back(schema[i]);
    }
}

void insert(vector<string> cmd)
{
    string table_name = cmd[2];
    if (!checkTable(table_name))
    {
        SetConsoleTextAttribute(console_color, 12);
        cout << "Error: Table Does Not Exists" << endl;
        SetConsoleTextAttribute(console_color, 15);
        return;
    }
    fstream table;
    table.open(table_name + ".txt", ios::app);

    vector<string> data;
    getDatatype(table_name, data);

    vector<string> schema;

    fetchSchema(cmd[2], schema);

    int cols = 0;
    for (int i = 1; i < schema.size(); i += 2)
    {
        cols++;
    }

    int cnt = 0;
    for(int i=0; i<cmd.size(); i++) 
    {
        if(cmd[i] != ",") 
        {
            cnt++;
        } 
    }

    if(cnt - 6 != cols) 
    {
        SetConsoleTextAttribute(console_color, 12);
        if(cnt - 6 > cols) 
        {
            cout<<"Error: Too many arguments provided to insert function\nRequired: "<<cols<<" Provided: "<<cnt - 6<<endl;
        }
        else 
        {
            cout<<"Error: Too few arguments provided to insert function\nRequired: "<<cols<<" Provided: "<<cnt - 6<<endl;
        }
        SetConsoleTextAttribute(console_color, 15);
        return;
    }

    int start = -1, end = -1;
    for (int i = 4; i < cmd.size(); i++)
    {
        if (cmd[i] == "(")
        {
            start = i;
        }
        if (cmd[i] == ")")
        {
            end = i;
        }
    }

    if (start == -1 || end == -1)
    {
        SetConsoleTextAttribute(console_color, 12);
        cout << "Error: Syntax Error" << endl;
        SetConsoleTextAttribute(console_color, 15);
        return;
    }
    int k = 0;
    bool invalidType = false;
    int st = start;

    while (st < end - 1)
    {
        st++;
        if (cmd[st] != ",")
        {
            if ((data[k] == "INT" && !is_number(cmd[st])) ||
                (data[k] != "INT" && is_number(cmd[st])))
            {
                SetConsoleTextAttribute(console_color, 12);
                cout << "Error: Invalid Data Type " << cmd[st] << endl;
                SetConsoleTextAttribute(console_color, 15);
                return;
            }
            k++;
        }
    }

    while (start < end - 1)
    {
        start++;
        if (cmd[start] != ",")
        {

            table << cmd[start];
            if (start != end - 1)
            {
                table << "#";
            }
            else
            {
                table << "\n";
            }
        }
    }
    SetConsoleTextAttribute(console_color, 10);
    cout << "Values inserted Successfully" << endl;
    SetConsoleTextAttribute(console_color, 15);
}

void table_number(map<string, int> &table, vector<string> schema)
{
    int cnt = 0;
    for (int j = 1; j < schema.size(); j += 2)
    {
        table[schema[j]] = cnt;
        cnt++;
    }
}

// Check column of table
void checkCommand(map<int, string> &column_change, vector<string> cmd, vector<string> schema)
{
    int i;
    map<string, int> table;
    table_number(table, schema);
    for (i = 3; i < cmd.size() && cmd[i] != "where"; i += 3)
    {
        int col = table[cmd[i]];
        column_change[col] = cmd[i + 2];
    }
}

// Update function
void update(vector<string> cmd)
{

    if (!checkTable(cmd[1]))
    {
        SetConsoleTextAttribute(console_color, 12);
        cout << "Error: Table Does Not Exists" << endl;
        SetConsoleTextAttribute(console_color, 15);
        return;
    }

    vector<string> schema;
    fetchSchema(cmd[1], schema);

    if (!schema.empty())
    {
        string table_name = cmd[1] + ".txt";
        fstream table;
        const char *c = table_name.c_str();
        table.open(c, ios::in);
        vector<string>::iterator it;
        it = find(cmd.begin(), cmd.end(), "where");
        map<int, string> column_change;
        checkCommand(column_change, cmd, schema);

        string line;
        vector<string> lineVec;
        int i, j = 0;
        int flag = 0;
        int count = 0;
        fstream temp;
        bool changed = false;
        temp.open("temp.txt", ios::out);
        // where condition is not in command
        if (it == cmd.end())
        {
            while (getline(table, line))
            {
                string tab = line;
                i = 0;
                string curr = "";
                while (i < tab.length())
                {
                    if (tab[i] == '#')
                    {
                        lineVec.push_back(curr);
                        curr = "";
                    }
                    else
                    {
                        curr += tab[i];
                    }
                    i++;
                }
                lineVec.push_back(curr);

                string new_line = "";
                map<int, string>::iterator itr;
                for (itr = column_change.begin(); itr != column_change.end(); itr++)
                {
                    lineVec[itr->first] = itr->second;
                }

                for (i = 0; i < lineVec.size() - 1; i++)
                {
                    new_line += lineVec[i];
                    new_line += "#";
                    changed = true;
                }
                new_line += lineVec[i];
                temp << new_line << endl;
                lineVec.clear();
            }
        }
        // where condition is in command
        else
        {
            while (getline(table, line))
            {
                string tab = line;
                i = 0;
                string curr = "";
                while (i < tab.length())
                {
                    if (tab[i] == '#')
                    {
                        lineVec.push_back(curr);
                        curr = "";
                    }
                    else
                    {
                        curr += tab[i];
                    }
                    i++;
                }
                lineVec.push_back(curr);

                int idx = 0;
                idx = it - cmd.begin();
                j = 0;
                for (i = 1; i < schema.size(); i += 2)
                {
                    if (cmd[idx + 1] == schema[i])
                    {
                        // Equality condition
                        if (cmd[idx + 2] == "=")
                        {
                            if (lineVec[j] == cmd[idx + 3])
                            {
                                flag = 1;
                                count++;
                            }
                        }
                        // Greater than condition
                        if (cmd[idx + 2] == ">")
                        {
                            if (lineVec[j] > cmd[idx + 3])
                            {
                                flag = 1;
                                count++;
                            }
                        }
                        // Greater than or equal to
                        if (cmd[idx + 2] == ">=")
                        {
                            if (lineVec[j] >= cmd[idx + 3])
                            {
                                flag = 1;
                                count++;
                            }
                        }
                        // smaller than condition
                        if (cmd[idx + 2] == "<")
                        {
                            if (lineVec[j] < cmd[idx + 3])
                            {
                                flag = 1;
                                count++;
                            }
                        }
                        // smaller than and equal to
                        if (cmd[idx + 2] == "<=")
                        {
                            if (lineVec[j] <= cmd[idx + 3])
                            {
                                flag = 1;
                                count++;
                            }
                        }
                        // Not equal to condition
                        if (cmd[idx + 2] == "!=")
                        {
                            if (lineVec[j] != cmd[idx + 3])
                            {
                                flag = 1;
                                count++;
                            }
                        }
                    }
                    j++;
                }
                // Not making any changes in the row
                if (flag != 1)
                {
                    temp << line << endl;
                    lineVec.clear();
                }
                // make changes in a row
                else
                {
                    changed = true;
                    string new_line = "";
                    map<int, string>::iterator itr;
                    for (itr = column_change.begin(); itr != column_change.end(); itr++)
                    {
                        lineVec[itr->first] = itr->second;
                    }

                    for (i = 0; i < lineVec.size() - 1; i++)
                    {
                        new_line += lineVec[i];
                        new_line += "#";
                    }
                    new_line += lineVec[i];
                    temp << new_line << endl;
                    lineVec.clear();
                }
                flag = 0;
            }
        }
        table.close();
        temp.close();
        remove(c);
        rename("temp.txt", c);
        if (changed)
        {
            SetConsoleTextAttribute(console_color, 10);
            cout << "Rows updated successfully" << endl;
            SetConsoleTextAttribute(console_color, 15);
        }
        else
        {
            SetConsoleTextAttribute(console_color, 12);
            cout << "Error: Value not Present" << endl;
            SetConsoleTextAttribute(console_color, 15);
        }
    }
}

void delete_(vector<string> cmd)
{

    vector<string> schema;
    if (!checkTable(cmd[2]))
    {
        SetConsoleTextAttribute(console_color, 12);
        cout << "Error: Table Does Not Exists" << endl;
        SetConsoleTextAttribute(console_color, 15);
        return;
    }
    fetchSchema(cmd[2], schema);
    int count = 0;

    if (!schema.empty())
    {
        if (cmd.size() == 3)
        {
            string table_name = cmd[2] + ".txt";
            fstream table;
            table.open(table_name, ios::out | ios::trunc);
            SetConsoleTextAttribute(console_color, 10);
            cout << "Table deleted" << endl;
            SetConsoleTextAttribute(console_color, 15);
            table.close();
        }
        else if (cmd[3] == "where")
        {
            string table_name = cmd[2];
            fstream table;
            table.open(table_name + ".txt", ios::in);
            string line;

            int i, j;
            j = 0;
            int flag = 0;
            vector<string> lineVec;

            fstream temp;
            temp.open("temp.txt", ios::out);

            while (getline(table, line))
            {
                stringstream ss(line);
                while (ss.good())
                {
                    string substr;
                    getline(ss, substr, '#');
                    lineVec.push_back(substr);
                }

                for (i = 1; i < schema.size(); i += 2)
                {
                    if (cmd[4] == schema[i])
                    {
                        if (cmd[5] == "=")
                        {
                            if (cmd[6] == lineVec[j])
                            {
                                flag = 1;
                                count++;
                            }
                        }
                        if (cmd[5] == ">")
                        {
                            if (lineVec[j] < cmd[6])
                            {
                                flag = 1;
                                count++;
                            }
                        }
                        if (cmd[5] == "<")
                        {
                            if (lineVec[j] < cmd[6])
                            {
                                flag = 1;
                                count++;
                            }
                        }
                        if (cmd[5] == ">=")
                        {
                            if (lineVec[j] >= cmd[6])
                            {
                                flag = 1;
                                count++;
                            }
                        }
                        if (cmd[5] == "<=")
                        {
                            if (lineVec[j] <= cmd[6])
                            {
                                flag = 1;
                                count++;
                            }
                        }
                        if (cmd[5] == "!=")
                        {
                            if (cmd[6] != lineVec[j])
                            {
                                flag = 1;
                                count++;
                            }
                        }
                    }
                    j++;
                }
                if (flag != 1)
                {
                    temp << line << endl;
                }
                flag = 0;
            }
            table.close();
            temp.close();
            string table1 = table_name + ".txt";
            char c[table1.size() + 1];
            strcpy(c, table1.c_str());
            remove(c);
            rename("temp.txt", c);
            if(count > 0) {
                SetConsoleTextAttribute(console_color, 10);
                cout << count << " rows deleted successfully" << endl;
                SetConsoleTextAttribute(console_color, 15);
            } else {
                SetConsoleTextAttribute(console_color, 12);
                cout << "No match found" << endl;
                SetConsoleTextAttribute(console_color, 15);
            }
        }
    }
}

void fetchTable(string &tableName, vector<string> &att, unordered_map<string, int> &mp,
                vector<vector<string>> &d, bool star)
{
    fstream table(tableName + ".txt", ios::in);
    string line;

    if(!star) 
    {
        cout<<endl;
        for(int i = 0; i <= att.size()*2; i++) 
        {
            cout<<"--------------";
        }
        cout << endl;

        for (auto x : att) 
        {
            cout << "\t\t\t" << x;
        }
        cout << endl;
        for(int i = 0; i <= att.size()*2; i++) {
            cout<<"--------------";
        }
        cout<<endl;
    }
    

    while (getline(table, line))
    {
        stringstream ss(line);
        vector<string> data;

        while (ss.good())
        {
            string substr;
            getline(ss, substr, '#');
            data.push_back(substr);
        }

        d.push_back(data);
    }
}

void select(vector<string> cmd)
{

    if (cmd[1] == "*")
    {
        vector<string> schema;
        fetchSchema(cmd[3], schema);
        cout<<endl;
        for(int i = 0; i < schema.size(); i++) {
            cout<<"--------------";
        }
        cout<<endl;
        for (int i = 1; i < schema.size(); i += 2)
        {
            cout << "\t\t\t" << schema[i];
        }
        cout<<endl;
        for(int i = 0; i < schema.size(); i++) {
            cout<<"--------------";
        }
        cout<<endl;
        string table_name = cmd[3];
        fstream table;
        table.open(table_name + ".txt", ios::in);
        string line;

        int i, j;
        j = 0;
        int flag = 0;
        vector<string> lineVec;

        fstream temp;
        temp.open("temp.txt", ios::out);

        while (getline(table, line))
        {
            stringstream ss(line);
            while (ss.good())
            {
                string substr;
                getline(ss, substr, '#');
                lineVec.push_back(substr);
            }
        }

        vector<string> attr_list;

        auto it = find(cmd.begin(), cmd.end(), "from");
        int index = 0; // finding the index of from clause

        if (it != cmd.end())
        {
            index = it - cmd.begin();
        }
        else
        {
            SetConsoleTextAttribute(console_color, 12);
            cout << "Error: Syntax error" << endl;
            SetConsoleTextAttribute(console_color, 15);
            return;
        }

        int tablename_pos = index + 1; // position of table_name

        if (!checkTable(cmd[tablename_pos]))
        {
            SetConsoleTextAttribute(console_color, 12);
            cout << "Error: Table Does Not Exists" << endl;
            SetConsoleTextAttribute(console_color, 15);
            return;
        }

        unordered_map<string, int> table_attr;
        schema.clear();

        fetchSchema(cmd[tablename_pos], schema);

        int k = 0, cols = 0;
        for (int i = 1; i < schema.size(); i += 2)
        {
            table_attr[schema[i]] = k++;
            attr_list.push_back(schema[i]);
            cols++;
        }
        // cout<<"cols: "<<cols<<endl;

        bool attError = false;

        for (auto x : attr_list)
        {
            if (table_attr.find(x) == table_attr.end())
            {
                attError = true;
                break;
            }
        }

        if (attError)
        {
            SetConsoleTextAttribute(console_color, 12);
            cout << "Error: Attribute Error" << endl;
            SetConsoleTextAttribute(console_color, 15);
            return;
        }

        vector<vector<string>> data;
        fetchTable(cmd[tablename_pos], attr_list, table_attr, data, true);

        auto itw = find(cmd.begin(), cmd.end(), "where");
        int indexofwhere = 0; // finding the index of where clause
        if (itw != cmd.end())
        {
            indexofwhere = itw - cmd.begin();
            string cnd_att = cmd[indexofwhere + 1];
            string cnd = cmd[indexofwhere + 2];
            string cnd_value = cmd[indexofwhere + 3];

            int att_ind = table_attr[cnd_att];

            for (int k = 0; k < data.size(); k++)
            {
                if ((cnd == "=" && data[k][att_ind] == cnd_value) || (cnd == ">" && data[k][att_ind] > cnd_value) || (cnd == "<" && data[k][att_ind] < cnd_value) || (cnd == ">=" && data[k][att_ind] >= cnd_value) || (cnd == "<=" && data[k][att_ind] <= cnd_value) || (cnd == "!=" && data[k][att_ind] != cnd_value))
                {
                    for (int i = 0; i < attr_list.size(); i++)
                    {
                        cout << data[k][table_attr[attr_list[i]]] << "\t";
                    }
                    cout << endl;
                }
            }
        }
        else
        {
            for (int i = 0; i < lineVec.size(); i++)
            {
                if(i != 0 && i%cols == 0) cout<<endl;
                cout << "\t\t\t" << lineVec[i];
            }
            cout<<endl;
        }
        for(int i = 0; i < schema.size(); i++) {
            cout<<"--------------";
        }
        cout<<endl;
    }

    else
    {
        vector<string> attr_list;

        auto it = find(cmd.begin(), cmd.end(), "from");
        int index = 0; // finding the index of from clause

        if (it != cmd.end())
        {
            index = it - cmd.begin();
        }
        else
        {
            SetConsoleTextAttribute(console_color, 12);
            cout << "Error: Syntax error" << endl;
            SetConsoleTextAttribute(console_color, 15);
            return;
        }

        int tablename_pos = index + 1; // position of table_name

        if (!checkTable(cmd[tablename_pos]))
        {
            SetConsoleTextAttribute(console_color, 12);
            cout << "Error: Table Not Exists" << endl;
            SetConsoleTextAttribute(console_color, 15);
            return;
        }

        for (int i = 1; i < index; i += 2)
        {
            attr_list.push_back(cmd[i]);
        }

        unordered_map<string, int> table_attr;
        vector<string> schema;

        fetchSchema(cmd[tablename_pos], schema);

        int k = 0;
        for (int i = 1; i < schema.size(); i += 2)
        {
            table_attr[schema[i]] = k++;
        }

        bool attError = false;

        for (auto x : attr_list)
        {
            if (table_attr.find(x) == table_attr.end())
            {
                attError = true;
                break;
            }
        }

        if (attError)
        {
            SetConsoleTextAttribute(console_color, 12);
            cout << "Error: Attribute Error" << endl;
            SetConsoleTextAttribute(console_color, 15);
            return;
        }

        vector<vector<string>> data;
        fetchTable(cmd[tablename_pos], attr_list, table_attr, data, false);

        auto itw = find(cmd.begin(), cmd.end(), "where");
        int indexofwhere = 0; // finding the index of where clause
        if (itw != cmd.end())
        {
            indexofwhere = itw - cmd.begin();
            string cnd_att = cmd[indexofwhere + 1];
            string cnd = cmd[indexofwhere + 2];
            string cnd_value = cmd[indexofwhere + 3];

            int att_ind = table_attr[cnd_att];

            for (int k = 0; k < data.size(); k++)
            {
                if ((cnd == "=" && data[k][att_ind] == cnd_value) || (cnd == ">" && data[k][att_ind] > cnd_value) || (cnd == "<" && data[k][att_ind] < cnd_value) || (cnd == ">=" && data[k][att_ind] >= cnd_value) || (cnd == "<=" && data[k][att_ind] <= cnd_value) || (cnd == "!=" && data[k][att_ind] != cnd_value))
                {
                    for (int i = 0; i < attr_list.size(); i++)
                    {
                        cout << "\t\t\t" << data[k][table_attr[attr_list[i]]];
                    }
                    cout << endl;
                }
            }
        }
        else
        {
            for (int k = 0; k < data.size(); k++)
            {
                for (int i = 0; i < attr_list.size(); i++)
                {
                    cout << "\t\t\t" << data[k][table_attr[attr_list[i]]];
                }
                cout << endl;
            }
        }
        for(int i = 0; i <= attr_list.size()*2; i++) {
            cout<<"--------------";
        }
        cout<<endl;
    }
    cout<<endl;
}

void helpTable()
{
    SetConsoleTextAttribute(console_color, 14);
    string line;
    schemafile.open("Schema.txt");

    int count = 0;
    while (getline(schemafile, line))
    {
        string l1 = line.substr(0, line.find('#'));
        cout << l1 << endl;
        count++;
    }
    if (count == 0)
    {
        cout << "No tables found" << endl;
    }
    cout << endl;
    SetConsoleTextAttribute(console_color, 15);
}

void helpCmd(vector<string> cmd)
{
    SetConsoleTextAttribute(console_color, 14);
    map<string, int> help{{"createtable", 1}, {"droptable", 2}, {"select", 3}, {"insert", 4}, {"delete", 5}, {"update", 6}};
    map<string, int>::iterator it1;
    string command = cmd[1] + (cmd.size() > 2 ? cmd[2] : "");
    it1 = help.find(command);

    switch (it1->second)
    {
    case 1:
        cout << "\nCommand : CREATE TABLE" << endl;
        cout << "Info: Creates a new table" << endl;
        cout << "\nFormat: \nCREATE TABLE table_name ( attribute_1 attribute1_type CHECK (constraint1), \nattribute_2 attribute2_type, …, PRIMARY KEY ( attribute_1, attribute_2 ), \nFOREIGN KEY ( attribute_y ) REFERENCES table_x ( attribute_t ), \nFOREIGN KEY ( attribute_w ) REFERENCES table_y ( attribute_z )...);" << endl;
        break;

    case 2:
        cout << "\nCommand : DROP TABLE" << endl;
        cout << "Info: Deletes a table" << endl;
        cout << "\nFormat: \nDROP TABLE table_name;" << endl;
        break;

    case 3:
        cout << "\nCommand : SELECT" << endl;
        cout << "Info: Extracts data from a database" << endl;
        cout << "\nFormat: \nSELECT attribute_list FROM table_list WHERE condition_list;" << endl;
        break;

    case 4:
        cout << "\nCommand : INSERT" << endl;
        cout << "Info: Inserts new data into a database" << endl;
        cout << "\nFormat: \nINSERT INTO table_name VALUES ( val1, val2, ... );" << endl;
        break;

    case 5:
        cout << "\nCommand : DELETE" << endl;
        cout << "Info: Deletes data from a database" << endl;
        cout << "\nFormat: \nDELETE FROM table_name WHERE condition_list;" << endl;
        break;

    case 6:
        cout << "\nCommand : UPDATE" << endl;
        cout << "Info: Updates data in a database" << endl;
        cout << "\nFormat: \nUPDATE table_name SET attr1 = val1, attr2 = val2… WHERE condition_list;" << endl;
        break;

    default:
        SetConsoleTextAttribute(console_color, 12);
        cout << "Error: Syntax Error"<<endl;
        SetConsoleTextAttribute(console_color, 15);
    }
    cout << endl;
    SetConsoleTextAttribute(console_color, 15);
}

void convertToVector(string input, vector<string> &v)
{
    string word = "";
    for (auto x : input)
    {
        if (x == ' ')
        {
            v.push_back(word);
            word = "";
        }
        else
        {
            word = word + x;
        }
    }
    v.push_back(word);
}

void fetchSchema(string tableName, vector<string> &schema)
{
    schemafile.open("Schema.txt", ios::in);
    string line;
    int flag = 0;

    while (getline(schemafile, line))
    {
        string l1 = line.substr(0, line.find('#'));
        if (tableName == l1)
        {
            flag = 1;
            stringstream ss(line);

            while (ss.good())
            {
                string substr;
                getline(ss, substr, '#');
                schema.push_back(substr);
            }
            break;
        }
    }
    if (flag == 0)
    {
        SetConsoleTextAttribute(console_color, 12);
        cout << "Error: Table not found" << endl;
        SetConsoleTextAttribute(console_color, 15);
    }

    schemafile.close();
}

void handleCmd(vector<string> cmd)
{
    if (cmd[0] == "create" && cmd[1] == "table")
    {
        createTable(cmd);
    }
    else if (cmd[0] == "drop" && cmd[1] == "table")
    {
        dropTable(cmd);
    }
    else if (cmd.size() == 2 && cmd[0] == "help" && cmd[1] == "tables")
    {
        helpTable();
    }
    else if (cmd.size() == 2 && cmd[0] == "help" && cmd[1] != "commands")
    {
        helpCmd(cmd);
    }
    else if (cmd[0] == "insert" && cmd[1] == "into")
    {
        insert(cmd);
    }
    else if (cmd[0] == "describe")
    {
        describe(cmd);
    }
    else if (cmd[0] == "select")
    {
        select(cmd);
    }
    else if (cmd[0] == "delete" && cmd[1] == "from")
    {
        delete_(cmd);
    }
    else if (cmd[0] == "update" && cmd[2] == "set")
    {
        update(cmd);
    }
    else
    {
        SetConsoleTextAttribute(console_color, 12);
        cout << "Error: Syntax Error"<<endl<<endl;
        SetConsoleTextAttribute(console_color, 15);
    }
}

int main()
{
    vector<string> cmd;
    string input;
    cout<<endl<<"DMSCP> ";
    getline(cin, input);

    while (input != "q")
    {
        convertToVector(input, cmd);
        // for(auto it=cmd.begin(); it!=cmd.end(); it++) cout<<*it<<endl;

        handleCmd(cmd);
        cmd.clear();

        cout<<"DMSCP> ";
        getline(cin, input);
    }

    return 0;
}
