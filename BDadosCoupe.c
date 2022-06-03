#include <stdint.h>
#include <time.h>
#include "BDadosCoupe.h"

#define FIXED_SIZE_ARRAY 50L
#define POINTERSIZE 8L
#define STATISTICS_FILE  "statistics.csv"


/** \brief Criar_BDados: A) Criar a Base de dados
 *
 * \param nome_bd char* : DB name
 * \param versao char*  : DB version
 * \return BDadosCoupe* : Pointer to created database
 *
 */
BDadosCoupe *Criar_BDados(char *nome_bd, char *versao) {
    BDadosCoupe *BD = (BDadosCoupe *) malloc(sizeof(BDadosCoupe));
    strcpy(BD->NOME_BDADOS, nome_bd);
    strcpy(BD->VERSAO_BDADOS, versao);
    BD->LTabelas = CriarLG();
    return BD;
}

/** \brief B) Creates a table inside DB
 *
 * \param BD BDadosCoupe* : Pointer to database
 * \param nome_tabela char* : table name
 * \return TABELA*        : Returns a pointer to the created table.
 * \author                :  AS,HS,PC
 */
TABELA *Criar_Tabela(BDadosCoupe *BD, char *nome_tabela) {
    TABELA *table = (TABELA *) malloc(sizeof(TABELA));
    //The table name is converted to UPPERCASE for normalize user input
    strcpy(table->NOME_TABELA, strupr(nome_tabela));
    table->LCampos = CriarLG();
    table->LRegistos = CriarLG();
    AddLG(BD->LTabelas, table);
    return table;
}

/** \brief   Add a field to the table
 *
 * \param T TABELA* : Table Pointer
 * \param nome_campo char* : Field name
 * \param tipo_campo char* : Field type (INT, STRING, CHAR, FLOAT)
 * \return int             : Returns SUCESSO/INSUCESSO
 */
int Add_Campo_Tabela(TABELA *T, char *nome_campo, char *tipo_campo) {
    if (!T) return INSUCESSO;
    CAMPO *campo = (CAMPO *) malloc(sizeof(CAMPO));
    strcpy(campo->NOME_CAMPO, nome_campo);
    strcpy(campo->TIPO, tipo_campo);
    return AddLG(T->LCampos, campo);
}

/**
 * \brief Add a new value to the table
 * @param T - Pointer table
 * @param dados: - String of data splitted with; ex 1;car;volvo
 * @return   : Returns SUCESSO/INSUCESSO
 */
int Add_Valores_Tabela(TABELA *T, char *dados) {
    if (!T)return INSUCESSO;
    if (dados) {
        char *tmp = (char *) malloc(sizeof(char) * (strlen(dados) + 1));
        strcpy(tmp, dados);
        char *data = strtok(tmp, ";");
        REGISTO *row = CriarLG();
        while (data != NULL) {
            char *registo = (char *) malloc(sizeof(char) * (strlen(data) + 1));
            strcpy(registo, data);
            data = strtok(NULL, ";");
            AddLG(row, registo);
        }
        AddLG(T->LRegistos, row);
        tmp = NULL;
        free(tmp);
        return SUCESSO;
    }
    return INSUCESSO;
}


/**
 * @param BD : Database name
 * @param nome_tabela :Name of table
 * @param dados :Data to be inserted in the table.\n
 * Note: Data must be a string with with values separated by character ;
 * @return : SUCESSO - Case of success\n
 *          :INSUCESSO - Case of fail
 */
int Add_Valores_Tabela_BDados(BDadosCoupe *BD, char *nome_tabela, char *dados) {
    if (!BD)return INSUCESSO;
    if (!nome_tabela)return INSUCESSO;
    if (!dados)return INSUCESSO;
    char *tbl_name = (char *) malloc(sizeof(char) * strlen(nome_tabela) + 1);
    strcpy(tbl_name, nome_tabela);
    TABELA *T = Pesquisar_Tabela(BD, strupr(tbl_name));
    free(tbl_name);
    if (T) {
        return Add_Valores_Tabela(T, dados);
    }
    return INSUCESSO;
}

/**
 * \brief: Search a table
 * @param BD : Database name
 * @param nome_tabela : Name of the table to search
 *  @return : Table pointer - Case of success found\n
 *          :NULL - Case of fail
 */
TABELA *Pesquisar_Tabela(BDadosCoupe *BD, char *nome_tabela) {
    if (!BD)return NULL;
    //Guard nome_tabela with at least one caracter
    if (strlen(nome_tabela) < 1) {
        printf("Nome da tabela invalido.Indique o nome da tabela!\n");
        return NULL;
    }
    //Iterate the list searching the table
    if (BD->LTabelas->NEL) {
        NOG *n = BD->LTabelas->Inicio;
        while (n) {
            TABELA *t = (TABELA *) n->Info;
            //Compare the name
            if (strcmp(nome_tabela, t->NOME_TABELA) == 0)return t;
            n = n->Prox;
        }
        return NULL;
    }
    return NULL;
}

/**
 * \brief Shows to the user the content of the table
 * @param BD : Database name
 * @param tabela : Pointer for a table to print
 */
void Mostrar_Tabela_NOME(BDadosCoupe *BD, char *tabela) {
    if (!BD)return;
    if (strlen(tabela) < 1) {
        printf("Nome da tabela invalido.Indique o nome da tabela!\n");
        return;
    }
    TABELA *t = Pesquisar_Tabela(BD, tabela);
    if (t != NULL) {
        Mostrar_Tabela(t);
    }
}

/**
 * \brief Show a table
 * @param T :Pointer for a table to print
 */
void Mostrar_Tabela(TABELA *T) {
    if (!T) return;
    //Print the hear
    printf("------------Tabela [%s]------------\n", T->NOME_TABELA);
    NOG *n = T->LCampos->Inicio;
    while (n) {
        CAMPO *c = (CAMPO *) n->Info;
        printf("[%s] (%s)\t", c->NOME_CAMPO, c->TIPO);
        n = n->Prox;
    }
    printf("\n");
    //Print the body content
    NOG *registerListNode = T->LRegistos->Inicio;
    while (registerListNode) {
        ListaGenerica *ab = registerListNode->Info;
        NOG *pointer = ab->Inicio;
        while (pointer) {
            printf("%s\t\t", (char *) pointer->Info);
            pointer = pointer->Prox;
        }
        printf("\n");
        registerListNode = registerListNode->Prox;
    }
    printf("-----[A tabela contem %d registo(s)]-----\n\n", T->LRegistos->NEL);
}

/**
 * \brief: Shows all the tables inside the database
 * @param BD :pointer to the database
 */
void Mostrar_BDados(BDadosCoupe *BD) {
    if (!BD)return;
    ListaGenerica *head = (ListaGenerica *) BD->LTabelas;
    NOG *node = head->Inicio;
    while (node) {
        TABELA *t = (TABELA *) node->Info;
        Mostrar_Tabela(t);
        node = node->Prox;
    }
}

/**
 * \brief : Remove all content and dealoc the database
 * @param BD :pointer to the database
 */
void Destruir_BDados(BDadosCoupe *BD) {
    if (!BD)return;
    if (BD->LTabelas->NEL > 0) {
        while (BD->LTabelas->NEL) {
            TABELA *inicio = BD->LTabelas->Inicio->Info;
            DROP_TABLE(BD, inicio->NOME_TABELA);
        }
    }
    free(BD->LTabelas);
    BD->LTabelas = NULL;
    printf("%s\n", BD->NOME_BDADOS);
    printf("Destruindo BD");
    free(BD);


}

/**
 * \brief: Shows how much memory the intire DB ocupies in BYTES
 * @param BD :pointer to the database
 * @return Bytes count
 */
long int Memoria_BDados(BDadosCoupe *BD) {
    if (!BD) return -1;
    long total_memory = 0;
    total_memory += sizeof(BDadosCoupe);
    ListaGenerica *tabelas = (ListaGenerica *) BD->LTabelas;
    NOG *node = tabelas->Inicio;
    //Iterate Table list
    while (node) {
        TABELA *t = (TABELA *) node->Info;
        total_memory += FIXED_SIZE_ARRAY + sizeof(NOG) + 2 * POINTERSIZE; // Tablename+Node+2 pointers to list
        printf("%d", sizeof(NOG *));
        // Iterates Table Fields for each table, since it can differ for every single one
        NOG *node_LCampos = t->LCampos->Inicio;
        while (node_LCampos) {
            total_memory += sizeof(CAMPO) + sizeof(NOG) +
                            POINTERSIZE; // Each field is a Node with CAMPO as info and pointer to next node Total: 60+16+8=84Bytes
            node_LCampos = node_LCampos->Prox;
        }
        //Iterates Entries, amount can also differ from table to table
        NOG *node_LRegistos = t->LRegistos->Inicio;
        while (node_LRegistos) {
            ListaGenerica *lregistos = node_LRegistos->Info;
            total_memory += sizeof(NOG);
            NOG *row = lregistos->Inicio;
            int i = 1;
            while (row) {
                total_memory += sizeof(NOG) + (strlen((char *) (row->Info)) + 1);
                i++;
                row = row->Prox;
            }
            node_LRegistos = node_LRegistos->Prox;
        }
        node = node->Prox;
    }

    printf("Memoria total da BD %ld BYTES \n", total_memory);
    return total_memory;
}

/**
 * \brief Shows the wasted memory caused by fixed array size inside some structs
 * @param BD :pointer to the database
 * @return Bytes count of wasted memory
 */
long int Memoria_Desperdicada_BDados(BDadosCoupe *BD) {
    if (!BD) return -1;
    long wasted_memory = 0;
    //Evaluate the memory used in BD head
    wasted_memory += (FIXED_SIZE_ARRAY - (strlen(BD->NOME_BDADOS) + 1));
    wasted_memory += (FIXED_SIZE_ARRAY - (strlen(BD->VERSAO_BDADOS) + 1));
    //Evaluate the memory in each ListTabelas - nome_tabela
    ListaGenerica *head = (ListaGenerica *) BD->LTabelas;
    NOG *node = head->Inicio;
    while (node) {
        TABELA *t = (TABELA *) node->Info;
        wasted_memory += (FIXED_SIZE_ARRAY - (strlen(t->NOME_TABELA) + 1));  // cada tabela tem o nome a 50
        NOG *node_LCampos = t->LCampos->Inicio;
        // Iterate the fields
        while (node_LCampos) {
            CAMPO *c = (CAMPO *) node_LCampos->Info;
            wasted_memory += (10 - (strlen(c->TIPO) + 1));
            wasted_memory += (FIXED_SIZE_ARRAY - (strlen(c->NOME_CAMPO) + 1));
            node_LCampos = node_LCampos->Prox;
        }
        node = node->Prox;
    }
    printf("Memoria desperdicada na BD %ld BYTES \n", wasted_memory);
    return wasted_memory;
}


/**
 * \brief : Export the table to CSV file
 * @param BD :Pointer to BD
 * @param tabela :Pointer to a table to export
 * @param ficheir_csv : file to export.Extension is added automaticly in case of missing
 * @return SUCESSO or INSUCESSO for the state of the import
 */
int Exportar_Tabela_BDados_Excel(BDadosCoupe *BD, char *tabela, char *ficheir_csv) {
    if (!BD) return -1;
    if (!tabela) return -1;
    if (!ficheir_csv) return -1;

    char extension[5] = ".csv";
    char *file_name = NULL;

    char *have_extension = strstr(ficheir_csv, ".csv");
    //Handle file passed with extension or just file name
    if (!have_extension) {
        //Passed file has no extension.We add it for you :-)
        printf("Nao tem  extensao, adicionando por si\n");
        file_name = (char *) malloc(sizeof(char) * strlen(ficheir_csv) + strlen(extension) + 1);
        strcpy(file_name, ficheir_csv);
        strcat(file_name, extension);
    } else {
        //Passed file has extension. All good to go :)
        file_name = (char *) malloc(sizeof(char) * strlen(ficheir_csv) + 1);
        strcpy(file_name, ficheir_csv);
    }
    FILE *ExpBD;
    ExpBD = fopen(file_name, "w");
    if (ExpBD) {
        TABELA *t = Pesquisar_Tabela(BD, tabela);
        if (t) {
            NOG *Aux = t->LCampos->Inicio;
            fprintf(ExpBD, "%s\n", t->NOME_TABELA);
            fprintf(ExpBD, "%d\n", t->LCampos->NEL);
            while (Aux) {
                CAMPO *C = (CAMPO *) Aux->Info;
                fprintf(ExpBD, "%s|%s;", C->NOME_CAMPO, C->TIPO);
                Aux = Aux->Prox;
            }
            fprintf(ExpBD, "\n");
            Aux = t->LRegistos->Inicio;
            fprintf(ExpBD, "%d\n", t->LRegistos->NEL);
            while (Aux) {
                ListaGenerica *R = (ListaGenerica *) Aux->Info;
                NOG *P = R->Inicio;
                while (P) {
                    fprintf(ExpBD, "%s;", (REGISTO *) P->Info);
                    P = P->Prox;
                }
                fprintf(ExpBD, "\n");
                Aux = Aux->Prox;
            }
            fclose(ExpBD);
            free(file_name);
            return SUCESSO;
        }
        printf("Esta tabela nao existe !!!\n");
        fclose(ExpBD);
        free(file_name);
        return INSUCESSO;
    }
    printf("Nao foi possivel escrever esse ficheiro\n");
    free(file_name);
    return INSUCESSO;
}

/**
 * \brief : Export the intire DB to CSV File
 * @param BD :Pointer to BD
 * @param ficheir_csv : file to export.Extension is added automaticly in case of missing
 * @return SUCESSO or INSUCESSO for the state of the import
 */
int Exportar_BDados_Excel(BDadosCoupe *BD, char *ficheir_csv) {
    if (!BD) return -1;
    if (!ficheir_csv) return -1;

    FILE *ExpBD;
    char extension[5] = ".csv";
    char *file_name = NULL;
    scanf("%s", ficheir_csv);
    char *have_extension = strstr(ficheir_csv, ".csv");
    //Handle file passed with extension or just file name
    if (!have_extension) {
        //Passed file has no extension.We add it for you :-)
        printf("Nao tem  extensao, adicionando por si\n");
        file_name = (char *) malloc(sizeof(char) * strlen(ficheir_csv) + strlen(extension) + 1);
        strcpy(file_name, ficheir_csv);
        strcat(file_name, extension);
    } else {
        //Passed file has extension. All good to go :)
        file_name = (char *) malloc(sizeof(char) * strlen(ficheir_csv) + 1);
        strcpy(file_name, ficheir_csv);
    }

    ExpBD = fopen(file_name, "w");
    if (ExpBD) {
        NOG *n = BD->LTabelas->Inicio;
        fprintf(ExpBD, "%s\n", BD->NOME_BDADOS);
        fprintf(ExpBD, "%s\n", BD->VERSAO_BDADOS);
        fprintf(ExpBD, "%d\n", BD->LTabelas->NEL);
        while (n) {
            TABELA *t = (TABELA *) n->Info;
            NOG *Aux = t->LCampos->Inicio;
            fprintf(ExpBD, "%s\n", t->NOME_TABELA);
            fprintf(ExpBD, "%d\n", t->LCampos->NEL);
            while (Aux) {
                CAMPO *C = (CAMPO *) Aux->Info;
                fprintf(ExpBD, "%s|%s;", C->NOME_CAMPO, C->TIPO);
                Aux = Aux->Prox;
            }
            fprintf(ExpBD, "\n");
            Aux = t->LRegistos->Inicio;
            fprintf(ExpBD, "%d\n", t->LRegistos->NEL);
            while (Aux) {
                ListaGenerica *R = (ListaGenerica *) Aux->Info;
                NOG *P = R->Inicio;
                while (P) {
                    fprintf(ExpBD, "%s;", (REGISTO *) P->Info);
                    P = P->Prox;
                }
                fprintf(ExpBD, "\n");
                Aux = Aux->Prox;
            }
            n = n->Prox;
            fprintf(ExpBD, "\n");
        }
        fclose(ExpBD);
        free(file_name);
        return SUCESSO;
    }
    free(file_name);
    return INSUCESSO;
}

/**
 * \brief Function to store execution performance statistics and log.Saves the statistics in \n statistics.csv
 * @param FunctionName :Name of the function to cron
 * @param start :Start timestampo
 * @param end :End timestamp
 */
void createStatistics(const char *FunctionName, clock_t start, clock_t end) {
    FILE *Time;
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    Time = fopen(STATISTICS_FILE, "a");
    double time_spent = (double) (end - start) / CLOCKS_PER_SEC;
    fprintf(Time, "%d-%02d-%02d %02d:%02d:%02d %s:%fms\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour,
            tm.tm_min, tm.tm_sec, FunctionName, time_spent);
    fclose(Time);
}

//private function for filter ;
char *scan(char **pp, char c) {
    char *s = *pp, *p;
    p = strchr(*pp, c);
    if (p) *p++ = '\0';
    *pp = p;
    return s;
}

//Typedef alias to string
typedef char *string;

/**
 * \brief Split a string with a ;
 * @param inputString :String yo split
 * @return : Returns an array with 2 string elements splited by the ;
 */
string *splitString(const char *inputString) {
    char *copy_string = malloc(sizeof(char) * strlen(inputString) + 1);
    string *container = (string *) malloc(sizeof(string) * 2);
    strcpy(copy_string, inputString);
    char delimiter[] = "|";
    int pos = 0;
    char *p = copy_string;
    while (p) {
        container[pos] = (char *) malloc(sizeof(char) * (strlen(p) + 1));
        strcpy(container[pos++], scan(&p, '|'));
    }
    free(copy_string);
    return container;
}

/**
 * \brief Allows the user to import a CSV file to restore de BD
 * @param BD : Pointer to BD
 * @param ficheir_csv : File CSV with the exported data
 * @return SUCESSO or INSUCESSO for the state of the import
 */
int Importar_BDados_Excel(BDadosCoupe *BD, char *ficheir_csv) {
    if (!BD) return INSUCESSO;
    if (!ficheir_csv) return INSUCESSO;

    FILE *ExpBD;

    char extension[5] = ".csv";
    char *file_name = NULL;
    char *have_extension = strstr(ficheir_csv, ".csv");
    //Handle file passed with extension or just file name
    if (!have_extension) {
        //Passed file has no extension.We add it for you :-)
        printf("Nao tem  extensao, adicionando por si\n");
        file_name = (char *) malloc(sizeof(char) * strlen(ficheir_csv) + strlen(extension) + 1);
        strcpy(file_name, ficheir_csv);
        strcat(file_name, extension);
    } else {
        //Passed file has extension. All good to go :)
        file_name = (char *) malloc(sizeof(char) * strlen(ficheir_csv) + 1);
        strcpy(file_name, ficheir_csv);
    }

    ExpBD = fopen(file_name, "r");
    if (ExpBD == NULL) {
        printf("Erro!\n");
        return INSUCESSO;
    }
    int nt = 0;

    fscanf(ExpBD, "%s", BD->NOME_BDADOS);
    //Read the database version
    fscanf(ExpBD, "%s", BD->VERSAO_BDADOS);
    //Read the number of tables stored
    fscanf(ExpBD, "%d", &nt);

    char NomeTABELA[50];
    char LerCampos[50];
    char LerRegistos[50];
    int nCampos;
    int nRegistos;


    for (int i = 0; i < nt; i++) {
        fscanf(ExpBD, "%s", NomeTABELA);
        TABELA *nomeTb = Criar_Tabela(BD, NomeTABELA);
        fscanf(ExpBD, "%d", &nCampos);
        fscanf(ExpBD, "%s", LerCampos);

        char *tmp = (char *) malloc(sizeof(char) * (strlen(LerCampos) + 1));
        strcpy(tmp, LerCampos);

        char *data = strtok(tmp, ";");
        while (data != NULL) {
            char *registo = (char *) malloc((strlen(data) + 1));
            strcpy(registo, data);
            data = strtok(NULL, ";");
            string *parts = splitString(registo);
            Add_Campo_Tabela(nomeTb, parts[0], parts[1]);
            free(parts);
            free(registo);
        }
        free(tmp);
        tmp = NULL;

        //Read the table data
        fscanf(ExpBD, "%d", &nRegistos);
        for (int b = 0; b < nRegistos; b++) {
            fscanf(ExpBD, "%s", LerRegistos);
            Add_Valores_Tabela(nomeTb, LerRegistos);
        }
    }

    fclose(ExpBD);
    free(file_name);
    return SUCESSO;
}

/**
 *
 *\brief Export the database for a binary file
 * @param BD - Datgabase name to use
 * @param fich_dat File name to write binary
 * @return SUCESSO or INSUCESSO for the state of the export
 */
int Exportar_BDados_Ficheiro_Binario(BDadosCoupe *BD, char *fich_dat) {
    if (!BD) return -1;
    if (!fich_dat) return -1;

    char extensiondat[5] = ".dat";
    char *file_namedat = NULL;
    char *have_extensiondat = strstr(fich_dat, ".dat");
    //Handle file passed with extension or just file name
    if (!have_extensiondat) {
        //Passed file has no extension.We add it for you :-)
        printf("Nao tem  extensao, adicionando por si\n");
        file_namedat = (char *) malloc(sizeof(char) * strlen(fich_dat) + strlen(extensiondat) + 1);
        strcpy(file_namedat, fich_dat);
        strcat(file_namedat, extensiondat);
    } else {
        //Passed file has extension. All good to go :)
        file_namedat = (char *) malloc(sizeof(char) * strlen(fich_dat) + 1);
        strcpy(file_namedat, fich_dat);
    }

    FILE *ExpBD;
    ExpBD = fopen(file_namedat, "wb");

    NOG *n = BD->LTabelas->Inicio;
    fwrite(BD->NOME_BDADOS, 50, 1, ExpBD);
    fwrite(BD->VERSAO_BDADOS, 50, 1, ExpBD);
    fwrite(&BD->LTabelas->NEL, sizeof(int), 1, ExpBD);
    while (n) {
        TABELA *t = (TABELA *) n->Info;
        NOG *Aux = t->LCampos->Inicio;
        fwrite(t->NOME_TABELA, 50, 1, ExpBD);
        fwrite(&t->LCampos->NEL, sizeof(int), 1, ExpBD);
        while (Aux) {
            CAMPO *C = (CAMPO *) Aux->Info;
            fwrite(C, sizeof(CAMPO), 1, ExpBD);
            Aux = Aux->Prox;
        }

        Aux = t->LRegistos->Inicio;
        fwrite(&t->LRegistos->NEL, sizeof(int), 1, ExpBD);
        while (Aux) {

            ListaGenerica *R = (ListaGenerica *) Aux->Info;
            NOG *x = R->Inicio;
            while (x) {
                int M = strlen(x->Info) + 1;
                fwrite(&M, sizeof(int), 1, ExpBD);
                fwrite(x->Info, sizeof(char), M, ExpBD);
                x = x->Prox;
            }
            Aux = Aux->Prox;
        }
        n = n->Prox;
    }
    fclose(ExpBD);
    free(file_namedat);
    return SUCESSO;
}

/**
 * \brief Import a binary file to restore the BD
 * @param BD : Pointer to BD
 * @param file :File to be loades
 * @return SUCESSO or INSUCESSO for the state of the import
 */
int Importar_BDados_Ficheiro_Binario(BDadosCoupe *BD, char *file) {

    if (!BD) return INSUCESSO;
    if (!file) return INSUCESSO;

    char extensiondat[5] = ".dat";
    char *fich_dat = NULL;
    char *have_extensiondat = strstr(file, ".dat");
    //Handle file passed with extension or just file name
    if (!have_extensiondat) {
        //Passed file has no extension.We add it for you :-)
        printf("Nao tem  extensao, adicionando por si\n");
        fich_dat = (char *) malloc(sizeof(char) * strlen(file) + strlen(extensiondat) + 1);
        strcpy(fich_dat, file);
        strcat(fich_dat, extensiondat);
    } else {
        //Passed file has extension. All good to go :)
        fich_dat = (char *) malloc(sizeof(char) * strlen(file) + 1);
        strcpy(fich_dat, file);
    }

    FILE *ExpBD;
    ExpBD = fopen(fich_dat, "rb");
    if (ExpBD == NULL) {
        printf("Erro!\n");
        exit(-1);
    }
    int nt = 0;
    char NomeTABELA[50];
    char LerRegistos[50];
    int nCampos;
    int nRegistos;
    int nCharReg;
    int contaT;

    fread(BD->NOME_BDADOS, 50, 1, ExpBD);
    fread(BD->VERSAO_BDADOS, 50, 1, ExpBD);
    fread(&nt, sizeof(int), 1, ExpBD);


    for (int ind_t = 0; ind_t < nt; ind_t++) {
        fread(NomeTABELA, 50, 1, ExpBD);
        TABELA *nomeTb = Criar_Tabela(BD, NomeTABELA);
        fread(&nCampos, sizeof(int), 1, ExpBD);
        for (int i = 0; i < nCampos; i++) {
            CAMPO C;
            fread(&C, sizeof(CAMPO), 1, ExpBD);
            Add_Campo_Tabela(nomeTb, C.NOME_CAMPO, C.TIPO);
        }
        fread(&nRegistos, sizeof(int), 1, ExpBD);
        contaT = nRegistos * nCampos;
        int Pos = 0;
//Init empty
        char BUFFER[100] = "";
        for (int int_r = 0; int_r < contaT; int_r++) {
            Pos++;
            fread(&nCharReg, sizeof(int), 1, ExpBD);
            fread(LerRegistos, sizeof(char), nCharReg, ExpBD);
            strcat(BUFFER, LerRegistos);
            strcat(BUFFER, ";");
            if (Pos == nCampos) {
                Add_Valores_Tabela(nomeTb, BUFFER);
                Pos = 0;
                strcpy(BUFFER, "");
            }
        }
    }
    fclose(ExpBD);
    free(fich_dat);
    return SUCESSO;
}

//Private function for dealoc a string and safe it to NULL
void destroy_info_string(void *info) {
    //this info is a string pointer
    free(info);
    //Force the info to be NULL - optional but cleaner and safer
    info = NULL;

}
//private function for pass in function
void destroy_info(void *info) {
    printf("Info is already free\n");
}
/**
 * \brief Delete the data inside the table.Does not remove table
 * @param T :table to empty
 * @return SUCESSO or INSUCESSO for empty result
 */
int DELETE_TABLE_DATA(TABELA *T) {
    if (!T)return INSUCESSO;
    NOG *registerListNode = T->LRegistos->Inicio;
    //Destroy the List inside each node->info
    while (registerListNode) {
        DestruirLG(registerListNode->Info, destroy_info_string);
        registerListNode = registerListNode->Prox;
    }
    //Destroy the List.The data (info) is already removed above
    //DestruirLG(T->LRegistos, destroy_info);
    DestruirLG(T->LRegistos, NULL);

    //Create a new List and add it empty
    T->LRegistos = CriarLG();
    return SUCESSO;
}

/**
 * \brief Delete the data field\n Warning
 * @param T :table to empty
 * @return SUCESSO or INSUCESSO for empty result
 */
int DELETE_TABLE_FIELDS(TABELA *T) {
    if (!T)return INSUCESSO;
    if (T->LCampos->NEL > 0) {
        NOG *node = T->LCampos->Inicio;
        while (node) {
            NOG *aux = node;
            free(aux->Info);
            node = aux->Prox;
            free(aux);
            T->LCampos->NEL--;
        }
        return SUCESSO;
    }
    return INSUCESSO;
}

/**
 * \brief Clear the content of the table and drop it.
 * @param BD :Pointer to BD
 * @param nome_tabela :Name of the table to drop
 * @return SUCESSO or INSUCESSO for empty result
 */
int DROP_TABLE(BDadosCoupe *BD, char *nome_tabela) {
    if (!BD)return INSUCESSO;
    if (strlen(nome_tabela) < 1)return INSUCESSO;
    TABELA *t = Pesquisar_Tabela(BD, nome_tabela);
    if (t) {
        //printf("Droping table %s\n", t->NOME_TABELA);
        NOG *P = BD->LTabelas->Inicio;
        NOG *Ant = NULL;
        int STOP = 0;
        while (P && !STOP) {
            if (P->Info == t) // if (func(P->Info, cc) == 1)
            {
                // Vou ter de remover o P
                STOP = 1;
            } else {
                Ant = P;
                P = P->Prox;
            }
        }
        // Agora ou encontrei ou n?o
        if (STOP == 1) {
            // Remover o que est? no P
            if (Ant)
                Ant->Prox = P->Prox;
            else // Caso do inicio
                BD->LTabelas->Inicio = P->Prox;

            DELETE_TABLE_DATA(t);
            DELETE_TABLE_FIELDS(t);
            free(P->Info);
            free(P);
            BD->LTabelas->NEL--;
            return SUCESSO;

        }
        return INSUCESSO;


        return SUCESSO;
    }
    return INSUCESSO;
}

//private function for print the content formated
void printResultNode(NOG *node) {
//Iterate all the nodes and print the info
    NOG *p = ((REGISTO *) (node->Info))->Inicio;
    while (p) {
        printf("%s\t\t", (char *) p->Info);
        p = p->Prox;
    }
    printf("\n");
}
//private function for print the table header
void showHeaderTable(NOG *node, char *title) {
    printf("+------------------ %s  QUERY---------------+\n", title);
    NOG *n = node;
    while (n) {
        CAMPO *c = (CAMPO *) n->Info;
        printf("[%s] (%s)\t", c->NOME_CAMPO, c->TIPO);
        n = n->Prox;
    }
    printf("\n");
}

/**
 * \brief Performs a select in a table with options
 * @param BD : Pointer to BD
 * @param _tabela :Name of the table to perform the select
 * @param f_condicao : User closure to run the filter
 * @param nome_campo : Name of the field to match
 * @param valor_comparacao : Value to compare
 * @return Prints the result to the user with affected rows
 */
int
SELECT(BDadosCoupe *BD, char *_tabela, int (*f_condicao)(char *, char *), char *nome_campo, char *valor_comparacao) {


    clock_t start, end;
    start = clock();

    if (!BD)return INSUCESSO;
    if (!strlen(_tabela))return INSUCESSO;
    if (!f_condicao)return INSUCESSO;
    if (!strlen(nome_campo))return INSUCESSO;
    if (!strlen(valor_comparacao))return INSUCESSO;


    TABELA *table = Pesquisar_Tabela(BD, _tabela);
    if (table) {
        //Check if the nome campo exists
        NOG *field = table->LCampos->Inicio;
        int i = 0;
        int field_found = 0;
        int field_pos;
        while (field) {
            if (strcmp((char *) field->Info, nome_campo) == 0) {
                field_found = 1;
                field_pos = i;
                break;
            }
            i++;
            field = field->Prox;
        }
        if (!field_found) {
            printf("\tNao foram encontrados registos com esse nome de campo!!!\n");
            return INSUCESSO;
        }

        //printf("Campo no indice %d\n\n", field_pos);
        showHeaderTable(table->LCampos->Inicio, "SELECT");
        int register_count = 0;
        //Pointer to the REGISTOS List
        NOG *node = table->LRegistos->Inicio;
        while (node) {
            NOG *p = ((REGISTO *) (node->Info))->Inicio;
            int pos = 0;
            while (p) {
                if (field_pos == pos) {
                    if (f_condicao((char *) p->Info, valor_comparacao)) {
                        register_count++;
                        printResultNode(node);
                    }
                }
                pos++;
                p = p->Prox;
            }
            node = node->Prox;
        }
        printf("Foram encontrados %d registos\n", register_count);

    }

    end = clock();
    createStatistics(__FUNCTION__, start, end);
    return SUCESSO;
}


/**
 * \brief Removes a REGISTER node from a linked List
 * @param l :List
 * @param element : Node element to remove
 * @return
 */
void removeRegister(ListaGenerica *l, NOG *element) {
    if (!l)return ;
    if (!element)return ;

    NOG *previous, *actual;
//    //Find the node to be removed
    actual = l->Inicio;
    while (actual) {
        if (actual == element) {
            if (l->Inicio == actual) {
                DestruirLG(actual->Info, destroy_info_string);
                l->NEL--;
                l->Inicio = actual->Prox;
            } else {
                DestruirLG(actual->Info, destroy_info_string);
                l->NEL--;
                previous->Prox = actual->Prox;
            }
        }
        previous = actual;
        actual = actual->Prox;
    }

}

/**
 *
 * @param BD : Pointer to BD
 * @param _tabela :Table to perfom the delete
 * @param f_condicao : User closure for filter
 * @param nome_campo : Name to the field
 * @param valor_comparacao :Value to compare
 * @return
 */
int
DELETE(BDadosCoupe *BD, char *_tabela, int (*f_condicao)(char *, char *), char *nome_campo, char *valor_comparacao) {
    //createStatistics metrics
    clock_t start, end;
    start = clock();

    if (!BD)return INSUCESSO;
    if (!strlen(_tabela))return INSUCESSO;
    if (!f_condicao)return INSUCESSO;
    if (!strlen(nome_campo))return INSUCESSO;
    if (!strlen(valor_comparacao))return INSUCESSO;

    TABELA *table = Pesquisar_Tabela(BD, _tabela);
    if (table) {
        //Check if the nome campo exists
        NOG *field = table->LCampos->Inicio;
        int i = 0;
        int field_found = 0;
        int field_pos;
        while (field) {
            if (strcmp((char *) field->Info, nome_campo) == 0) {
                field_found = 1;
                field_pos = i;
                break;
            }
            i++;
            field = field->Prox;
        }
        if (!field_found) {
            printf("\tNao foram encontrados registos com esse nome de campo!!!\n");
            return INSUCESSO;
        }

        showHeaderTable(table->LCampos->Inicio, "DROP");
        int register_count = 0;
        //Pointer to the REGISTOS List
        NOG *node = table->LRegistos->Inicio;
        int found = 0;
        while (node) {
            NOG *p = ((REGISTO *) (node->Info))->Inicio;
            int pos = 0;
            while (p) {
                if (field_pos == pos) {
                    if (f_condicao((char *) p->Info, valor_comparacao)) {
                        printResultNode(node);
                        found = 1;
                        register_count++;
                    }
                }
                pos++;
                p = p->Prox;
            }
            if (found) {
                removeRegister(table->LRegistos, node);
                found = 0;
            }
            node = node->Prox;
        }
        printf("+------------------------------------------+\n");
        printf("Foram encontrado(s) %d registos\n\n\n", register_count);

    }
    end = clock();
    createStatistics(__FUNCTION__, start, end);
    return SUCESSO;
}

/**
 * \brief Update a REGISTER node with a new value based on the field_position
 * @param r : Register node to uptade
 * @param field_pos : Field position to update
 * @param newValue : Value to update
 */
void updateREGISTER(NOG *r, int field_pos, char *newValue) {
    if (!r)return;
    if (!newValue)return;
    int pos = 0;

    NOG *p = ((REGISTO *) (r->Info))->Inicio;
    while (p) {
        if (field_pos == pos) {
            free(p->Info);
            p->Info = (char *) malloc(sizeof(char) * strlen(newValue) + 1);
            strcpy(p->Info, newValue);
            return;
        }
        pos++;
        p = p->Prox;
    }
}

/**
 * \brief Perform a SQL UPDATE to the BD with somo options
 * @param BD : Pointer to bD
 * @param _tabela : Name of the table to perfomr the query
 * @param f_condicao : User closure to filter
 * @param campo_comp : Name of the field
 * @param valor_campo_comp : Value of the field
 * @param nome_campo_update : Name of field to update
 * @param valor_campo_update : Value to update
 * @return
 */
int UPDATE(BDadosCoupe *BD, char *_tabela, int (*f_condicao)(char *, char *), char *campo_comp, char *valor_campo_comp,
           char *nome_campo_update, char *valor_campo_update) {

    clock_t start, end;
    start = clock();

    if (!BD)return INSUCESSO;
    if (!strlen(_tabela))return INSUCESSO;
    if (!f_condicao)return INSUCESSO;
    if (!strlen(campo_comp))return INSUCESSO;
    if (!strlen(valor_campo_comp))return INSUCESSO;

    TABELA *table = Pesquisar_Tabela(BD, _tabela);
    if (table) {
        //Check if the nome campo exists
        NOG *field = table->LCampos->Inicio;
        int i = 0;
        int field_found = 0;
        int field_pos = 0;
        while (field) {
            if (strcmp((char *) field->Info, campo_comp) == 0) {
                field_found = 1;
                field_pos = i;
                break;
            }
            i++;
            field = field->Prox;
        }
        //Check if the nome_campo_update exists
        i = 0;
        field_found = 0;
        int field_update_pos = 0;
        field = table->LCampos->Inicio;
        while (field) {
            if (strcmp((char *) field->Info, nome_campo_update) == 0) {
                field_found = 1;
                field_update_pos = i;
                break;
            }
            i++;
            field = field->Prox;
        }

        if (!field_found) {
            printf("\tNao foram encontrados registos com esse nome de campo!!!\n");
            return INSUCESSO;
        }
        printf("Campo aprocurar no indice %d\n\n", field_pos);
        printf("Campo a atualizar no indice %d\n\n", field_update_pos);

        showHeaderTable(table->LCampos->Inicio, "UPDATE");
        int register_count = 0;
        //Pointer to the REGISTOS List
        NOG *node = table->LRegistos->Inicio;
        while (node) {
            NOG *p = ((REGISTO *) (node->Info))->Inicio;
            int pos = 0;
            while (p) {
                if (field_pos == pos) {
                    if (f_condicao((char *) p->Info, valor_campo_comp)) {
                        updateREGISTER(node, field_update_pos, valor_campo_update);
                        register_count++;
                        printResultNode(node);
                    }
                }
                pos++;
                p = p->Prox;
            }
            node = node->Prox;
        }
        printf("+------------------------------+\n");
        printf("Foram encontrado(s) %d registo(s)\n", register_count);

    } else {
        printf("Tabela nao encontrada!!!\n");
    }
    end = clock();
    createStatistics(__FUNCTION__, start, end);
    return SUCESSO;
}



