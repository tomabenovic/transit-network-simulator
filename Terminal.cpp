#include "Terminal.h"

Terminal::Terminal(){}

Terminal& Terminal::getInstance()
{
    static Terminal terminal;
    return terminal;
}

void Terminal::openTerminal(){

    cout << "Dobrodosli u simulator mreze gradskog prevoza. Molimo Vas, odaberite opciju:" << endl;
    int n;
    do {
        cout << "1. Ucitavanje podataka o mrezi gradskog prevoza" << endl << "0. Kraj rada" << endl;
        cin >> n;
        if ((n != 0 and n != 1) || cin.fail()) cout << "Opcija koju ste uneli nije ispravna. Molimo Vas, odaberite opciju ponovo!" << endl << endl;
    } while (n != 0 && n != 1);

    if (n == 0) return;
    cin.ignore();

    Network::getInstance();

    bool flag = true;
    do {
        try {
            cout << "Molimo Vas, unesite putanju do fajla sa stajalistima ili kliknite ENTER za ucitavanje podrazumevanog fajla(ulazi / stajalista.txt) : ";
            string filename;
            getline(cin, filename);
            if (filename.empty()) {
                Network::getInstance().loadGraph("stajalista.txt");
                flag = false;
            }
            else {
                Network::getInstance().loadGraph(filename);
                flag = false;
            }
        }
        catch (FileNotFound* e) {
            cout << e->what() << endl;
        }
    } while (flag);

    flag = true;
    do {
        try {
            cout << "Molimo Vas, unesite putanju do fajla sa linijama ili kliknite ENTER za ucitavanje podrazumevanog fajla(ulazi / stajalista.txt) : ";
            string filename;
            getline(cin, filename);
            if (filename.empty()) {
                Network::getInstance().loadLines("linije.txt");
                flag = false;
            }
            else {
                Network::getInstance().loadLines(filename);
                flag = false;
            }
        }
        catch (FileNotFound* e) {
            cout << e->what() << endl;
        }
    } while (flag);

    cout << endl << endl << "Mreza gradskog prevoza je uspesno ucitana, molimo Vas da izaberete opciju: " << endl << endl;
    while (true) {
        cout << "1. prikaz informacija o stajalistu" << endl;
        cout << "2. prikaz informacija o liniji" << endl;        
        cout << "3. pronalazak putanje izmedju 2 stajalista" << endl;
        cout << "0. Kraj rada" << endl;
        cin >> n;
        switch (n) {
            case 0:
                //Network::getInstance().~Network();
                return;
            case 1:
                flag = true;
                while (flag) {
                    try {
                        cout << endl << "Unesite ID stajalista cije informacije zelite da vidite: ";
                        int id;
                        cin >> id;
                        Network::getInstance().printStationInformations(id);
                        flag = false;
                    }
                    catch (StopNotFound* e) {
                        cout << e->what() << endl;
                    }
                }
                break;
            case 2:
                cin.ignore();
                flag = true;
                while (flag) {
                    try {
                        cout << endl << "Unesite ime linije cije informacije zelite da vidite: ";
                        string name;
                        getline(cin, name);
                        Network::getInstance().printLineInformations(name);
                        flag = false;
                    }
                    catch (LineNotFound* e) {
                        cout << e->what() << endl;
                    }
                }
                break;
            case 3:
                cout << "Izaberite od sledecih strategija:" << endl;
                cout << "1. bilo koja putanja izmedju dve stanice" << endl;
                cout << "2. putanja sa najmanjim brojem presedanja izmedju dve stanice" << endl;
                cout << "3. putanja koja povezuje sva vazna stajalista" << endl;
            
                int choice;
                cin >> choice;
                switch (choice) {
                case 1: case 2:
                    flag = true;
                    while (flag) {
                        try {
                            cout << "Unesite ID pocetne i krajnje stanice: ";
                            int start, target;
                            cin >> start;
                            cin >> target;
                            Network::getInstance().minTransfersPath(start, target);
                            flag = false;
                        }
                        catch (StopNotFound* e) {
                            cout << e->what() << endl;
                        }
                    }
                    break;
                case 3:
                    Network::getInstance().throughAllImportantsPath();
                    break;
                default:
                    cout << "Opcija koju ste izabrali nije ispravna, molim Vas pokusajte ponovo!" << endl;
                    
                }
                break;
            default:
                cout << "Opcija koju ste izabrali nije ispravna, molim Vas pokusajte ponovo!" << endl;
            }
    }
}
