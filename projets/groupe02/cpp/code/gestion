#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include <ctime>
#include <iomanip>
#include <algorithm>
#include <sstream>

using namespace std;

// Déclarations anticipées
class Course;
class User;
class Student;
class Teacher;
class Secretariat;

// Classe Course
class Course {
public:
    string code;
    string name;
    int credits;
    vector<string> prerequisites;

    Course(string c, string n, int cr, vector<string> pre)
        : code(c), name(n), credits(cr), prerequisites(pre) {}
};

// Structures de données globales
vector<Course> courses;
map<string, User*> users;
map<string, vector<string>> registrations;
map<pair<string, string>, double> grades;

// Classe de base pour les utilisateurs
class User {
protected:
    string id;
    string password;
    string role;

public:
    User(string i, string pwd, string r) : id(i), password(pwd), role(r) {}
    virtual ~User() {}

    virtual void showMenu() = 0;

    bool authenticate(string pwd) {
        return password == pwd;
    }

    string getRole() { return role; }
    string getId() { return id; }
};

// Classe pour les étudiants
class Student : public User {
    vector<string> completedCourses;

public:
    Student(string i, string pwd) : User(i, pwd, "student") {
        ifstream file("students/" + id + ".txt");
        string course;
        while (getline(file, course)) {
            completedCourses.push_back(course);
        }
    }

    void showMenu() override {
        int choice;
        do {
            cout << "\nMenu Étudiant:\n";
            cout << "1. Consulter les UE disponibles\n";
            cout << "2. S'inscrire aux UE\n";
            cout << "3. Voir mes inscriptions\n";
            cout << "4. Télécharger fiche d'inscription\n";
            cout << "5. Consulter résultats\n";
            cout << "0. Déconnexion\n";
            cin >> choice;

            switch (choice) {
                case 1: viewCourses(); break;
                case 2: registerCourses(); break;
                case 3: viewRegistrations(); break;
                case 4: downloadFiche(); break;
                case 5: viewGrades(); break;
            }
        } while (choice != 0);
    }

    void viewCourses() {
        cout << "\nUE Disponibles:\n";
        for (auto& course : courses) {
            cout << course.code << " - " << course.name
                 << " (" << course.credits << " ECTS)\n";
            cout << "Prérequis: ";
            for (auto& pre : course.prerequisites)
                cout << pre << " ";
            cout << "\n\n";
        }
    }

    void registerCourses() {
        vector<string> selected;
        int totalCredits = 0;
        string code;

        while (true) {
            cout << "Code UE (ou 'fin' pour terminer): ";
            cin >> code;
            if (code == "fin") break;

            auto it = find_if(courses.begin(), courses.end(),
                [&](const Course& c) { return c.code == code; });

            if (it != courses.end()) {
                if (checkPrerequisites(*it) &&
                    (totalCredits + it->credits) <= 30) {
                    selected.push_back(code);
                    totalCredits += it->credits;
                } else {
                    cout << "Inscription impossible!\n";
                }
            }
        }

        registrations[id] = selected;
        saveRegistrations();
    }

    bool checkPrerequisites(const Course& course) {
        for (auto& pre : course.prerequisites) {
            if (find(completedCourses.begin(), completedCourses.end(), pre) == completedCourses.end()) {
                return false;
            }
        }
        return true;
    }

    void saveRegistrations() {
        ofstream file("registrations/" + id + ".csv");
        for (auto& code : registrations[id]) {
            file << code << "\n";
        }
    }

    void viewRegistrations() {
        cout << "\nInscriptions pour " << id << ":\n";
        for (auto& code : registrations[id]) {
            cout << code << "\n";
        }
    }

    void downloadFiche() {
        ofstream file("fiche_" + id + ".txt");
        file << "Fiche d'inscription - " << id << "\n";
        for (auto& code : registrations[id]) {
            file << code << "\n";
        }
        cout << "Fiche enregistrée.\n";
    }

    void viewGrades() {
        cout << "\nRésultats pour " << id << ":\n";
        for (auto& code : registrations[id]) {
            auto key = make_pair(id, code);
            if (grades.find(key) != grades.end()) {
                cout << code << ": " << grades[key] << "/20\n";
            } else {
                cout << code << ": Note non disponible\n";
            }
        }
    }
};

// Classe pour les enseignants
class Teacher : public User {
public:
    Teacher(string i, string pwd) : User(i, pwd, "teacher") {}

    void showMenu() override {
        int choice;
        do {
            cout << "\nMenu Enseignant:\n";
            cout << "1. Saisir les notes\n";
            cout << "0. Déconnexion\n";
            cin >> choice;

            switch (choice) {
                case 1: enterGrades(); break;
            }
        } while (choice != 0);
    }

    void enterGrades() {
        string courseCode;
        cout << "Code UE: ";
        cin >> courseCode;

        for (auto& student : registrations) {
            if (find(student.second.begin(), student.second.end(), courseCode) != student.second.end()) {
                double grade;
                cout << "Note pour " << student.first << ": ";
                cin >> grade;
                grades[{student.first, courseCode}] = grade;
            }
        }
        saveGrades();
    }

    void saveGrades() {
        ofstream file("grades.csv");
        for (auto& entry : grades) {
            file << entry.first.first << ","
                 << entry.first.second << ","
                 << entry.second << "\n";
        }
    }
};

// Classe pour le secrétariat
class Secretariat : public User {
public:
    Secretariat(string i, string pwd) : User(i, pwd, "secretariat") {}

    void showMenu() override {
        int choice;
        do {
            cout << "\nMenu Secrétariat:\n";
            cout << "1. Créer un cours\n";
            cout << "2. Générer un planning\n";
            cout << "0. Déconnexion\n";
            cin >> choice;

            switch (choice) {
                case 1: createCourse(); break;
                case 2: generateSchedule(); break;
            }
        } while (choice != 0);
    }

    void createCourse() {
        string code, name;
        int credits;
        vector<string> prerequisites;

        cout << "Nouvel UE:\nCode: ";
        cin >> code;
        cout << "Nom: ";
        cin.ignore();
        getline(cin, name);
        cout << "Crédits: ";
        cin >> credits;

        ofstream courseFile("courses.csv", ios::app);
        courseFile << code << "," << name << "," << credits << ",";
        for (auto& pre : prerequisites) courseFile << pre << ";";
        courseFile << "\n";

        courses.emplace_back(code, name, credits, prerequisites);
    }

    void generateSchedule() {
        time_t now = time(0);
        ofstream file("plannings/schedule_" + to_string(now) + ".txt");
        file << "Planning des cours:\n\n";
        for (auto& course : courses) {
            file << course.code << " - " << course.name << " | Lundi 10h-12h\n";
        }
    }
};

// Fonctions de gestion des fichiers
void loadData() {
    ifstream courseFile("courses.csv");
    string line;
    while (getline(courseFile, line)) {
        stringstream ss(line);
        string code, name, cred, pre;
        getline(ss, code, ',');
        getline(ss, name, ',');
        getline(ss, cred, ',');
        getline(ss, pre);

        vector<string> prerequisites;
        stringstream preSS(pre);
        string item;
        while (getline(preSS, item, ';')) {
            if (!item.empty()) prerequisites.push_back(item);
        }

        courses.emplace_back(code, name, stoi(cred), prerequisites);
    }

    ifstream userFile("users.csv");
    while (getline(userFile, line)) {
        stringstream ss(line);
        string id, pwd, role;
        getline(ss, id, ',');
        getline(ss, pwd, ',');
        getline(ss, role);

        if (role == "student") {
            users[id] = new Student(id, pwd);
        } else if (role == "teacher") {
            users[id] = new Teacher(id, pwd);
        } else {
            users[id] = new Secretariat(id, pwd);
        }
    }
}

// Fonction principale
int main() {
    loadData();

    string id, pwd;
    cout << "Système de Gestion Pédagogique\n";
    cout << "Identifiant: ";
    cin >> id;
    cout << "Mot de passe: ";
    cin >> pwd;

    if (users.find(id) != users.end() && users[id]->authenticate(pwd)) {
        users[id]->showMenu();
    } else {
        cout << "Authentification échouée!\n";
    }

    for (auto& u : users) delete u.second;
    return 0;
}
