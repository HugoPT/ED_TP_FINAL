
#include <stdint.h>
#include <ctype.h>


#include "BDadosCoupe.h"

#define FIXED_SIZE_ARRAY 50L
#define POINTERSIZE 8L

/** \brief Criar_BDados: A) Criar a Base de dados
 *
 * \param nome_bd char* : Nome da Base de Dados
 * \param versao char*  : Vers�o da Base de Dados
 * \return BDadosCoupe* : Ponteiro para a Base de Dados Criada
 *
 */
BDadosCoupe *Criar_BDados(char *nome_bd, char *versao) {
    BDadosCoupe *BD = (BDadosCoupe *) malloc(sizeof(BDadosCoupe));
    strcpy(BD->NOME_BDADOS, nome_bd);
    strcpy(BD->VERSAO_BDADOS, versao);
    BD->LTabelas = CriarLG();
    return BD;
}

/** \brief B) Criar uma Tabela na Base de Dados,
 * a tabela deve ser inserida � lista de tabelas da BD
 *
 * \param BD BDadosCoupe* : Ponteiro para a Base de Dados
 * \param nome_tabela char* : Nome da tabela a Criar
 * \return TABELA*        : Retorna a Tabela Criada
 * \author                : CS, FM & JL
 */
TABELA *Criar_Tabela(BDadosCoupe *BD, char *nome_tabela) {
    TABELA *table = (TABELA *) malloc(sizeof(TABELA));
//The table name is converted to UPPERCASE
    strcpy(table->NOME_TABELA, strupr(nome_tabela));
    table->LCampos = CriarLG();
    table->LRegistos = CriarLG();
    AddLG(BD->LTabelas, table);
    return table;
}

/** \brief C)  Adicionar um campo a uma tabela.
 *
 * \param T TABELA* : Ponteiro para a tabela
 * \param nome_campo char* : Nome do campo
 * \param tipo_campo char* : Tipo do Campo (INT, STRING, CHAR, FLOAT)
 * \return int             : SUCESSO/INSUCESSO
 * \author                 : CS, FM & JL              :
 * \version                : Versao 1.0
 * \date                   : 23/04/2022
 */
int Add_Campo_Tabela(TABELA *T, char *nome_campo, char *tipo_campo) {
    if (!T) return INSUCESSO;
    CAMPO *campo = (CAMPO *) malloc(sizeof(CAMPO));
    strcpy(campo->NOME_CAMPO, nome_campo);
    strcpy(campo->TIPO, tipo_campo);
    return AddLG(T->LCampos, campo);
}

//D)	Adicionar dados(registos) a uma tabela, os dados s�o dados numa string onde o separador � �;�m ex: Add_Valores_Tabela(T, �123;Joao;965654449�)
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

//F)	Pesquisar uma Tabela da base de dados
TABELA *Pesquisar_Tabela(BDadosCoupe *BD, char *nome_tabela) {
    if (!BD)return NULL;
    if (strlen(nome_tabela) < 1) {
        printf("Nome da tabela invalido.Indique o nome da tabela!\n");
        return NULL;
    }
    //Iterate the list
    if (BD->LTabelas->NEL) {
        NOG *n = BD->LTabelas->Inicio;
        while (n) {
            TABELA *t = (TABELA *) n->Info;
            // printf("%s\n",t->NOME_TABELA);
            if (strcmp(nome_tabela, t->NOME_TABELA) == 0)return t;
            n = n->Prox;
        }
        return NULL;
    }
    return NULL;
}

//G)	Mostrar uma Tabela
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

void Mostrar_Tabela(TABELA *T) {
    if (!T) return;
    //if (!T->LCampos->NEL)return;
    printf("------------Tabela [%s]------------\n", T->NOME_TABELA);
    NOG *n = T->LCampos->Inicio;
    while (n) {
        CAMPO *c = (CAMPO *) n->Info;
        printf("[%s] (%s)\t", c->NOME_CAMPO, c->TIPO);
        n = n->Prox;
    }
    printf("\n");


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
    printf("-----[A tabela contem %d registos]-----\n\n", T->LRegistos->NEL);
}

//H)	Mostrar toda a base de dados, dever� mostrar todas as Tabelas da BDados.
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

//I)	Libertar toda a mem�ria alocada pela base de dados.
void Destruir_BDados(BDadosCoupe *BD) {
    if (!BD)return;
    if (BD->LTabelas->NEL > 0) {
        while (BD->LTabelas->NEL) {
            TABELA *inicio = BD->LTabelas->Inicio->Info;
            printf(" Destruindo %s\n", inicio->NOME_TABELA);
            DROP_TABLE(BD, inicio->NOME_TABELA);
        }
    }
    free(BD->LTabelas);
    BD->LTabelas = NULL;
    printf("%s\n", BD->NOME_BDADOS);
    printf("Destruindo BD");
    free(BD);


}

//J)	Mem�ria ocupada por toda a base de dados.
//J)	Mem�ria ocupada por toda a base de dados.
//J)	Mem�ria ocupada por toda a base de dados.
long int Memoria_BDados(BDadosCoupe *BD) {
    if (!BD) return -1;
    long total_memory = 0;
    //Total memory used in BD 112Bytes
    total_memory += sizeof(BDadosCoupe);
    ListaGenerica *tabelas = (ListaGenerica *) BD->LTabelas;
    NOG *node = tabelas->Inicio;
    //Iterates Table list
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
        while (node_LCampos) {  // Iterar os campos
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

//K)	Exportar/Importar para/de Ficheiro (o retorno destas fun��es, permite saber se a fun��o foi bem/mal-executada!):
int Exportar_Tabela_BDados_Excel(BDadosCoupe *BD, char *tabela, char *ficheir_csv) {
    if (!BD) return -1;
    if (!tabela) return -1;
    if (!ficheir_csv) return -1;

    //Check if table exists.
    TABELA *found_table = Pesquisar_Tabela(BD, tabela);
    if (found_table) {
        int i = 0;
        char extension[5] = ".csv";
        char *file_name = NULL;
        char *have_extension = strstr(ficheir_csv, ".csv");
        //Handle file passed with extension or just file name
        if (!have_extension) {
            //Passed file has no extension.We add it for you :-)
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
            NOG *Aux = found_table->LCampos->Inicio;
            while (Aux) {
                fprintf(ExpBD, "%s;", (char *) Aux->Info);
                Aux = Aux->Prox;
            }
            fprintf(ExpBD, "\n");
            Aux = found_table->LRegistos->Inicio;
            while (Aux) {
                fprintf(ExpBD, "%s;", (char *) Aux->Info);
                Aux = Aux->Prox;
                i++;
                if (i == found_table->LCampos->NEL) {
                    fprintf(ExpBD, "\n");
                    i = 0;
                }
            }
            fprintf(ExpBD, "\n");
            fclose(ExpBD);
            free(file_name);
            return SUCESSO;
        }
        free(file_name);
        return INSUCESSO;
    }
    return INSUCESSO;
}


int Exportar_BDados_Excel(BDadosCoupe *BD, char *ficheir_csv) {
    if (!BD) return -1;
    if (!ficheir_csv) return -1;

    int i = 0;
    char extension[5] = ".csv";
    char *file_name = NULL;
    char *have_extension = strstr(ficheir_csv, ".csv");
    //Handle file passed with extension or just file name
    if (!have_extension) {
        //Passed file has no extension.We add it for you :-)
        printf("Nao tem  extensao\n");
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

//int Importar_BDados_Excel(BDadosCoupe *BD, char *ficheir_csv) {
//    if (!BD) return INSUCESSO;
//    if (!ficheir_csv) return INSUCESSO;
//
//    char extension[5] = ".csv";
//    char *file_name = NULL;
//    char *have_extension = strstr(ficheir_csv, ".csv");
//    //Handle file passed with extension or just file name
//    if (!have_extension) {
//        //Passed file has no extension.We add it for you ?
//        printf("Nao tem  extensao\n");
//        file_name = (char *) malloc(sizeof(char) * strlen(ficheir_csv) + strlen(extension) + 1);
//        strcpy(file_name, ficheir_csv);
//        strcat(file_name, extension);
//    } else {
//        //Passed file has extension. All good to go
//        file_name = (char *) malloc(sizeof(char) * strlen(ficheir_csv) + 1);
//        strcpy(file_name, ficheir_csv);
//    }
//    FILE *ExpBD;
//
//    ExpBD = fopen(file_name, "r");
//    if (ExpBD == NULL) {
//        printf("Erro!\n");
//        exit(-1);
//    }
//    int nt = 0;
//
//    fscanf(ExpBD, "%s", BD->NOME_BDADOS);
//    fscanf(ExpBD, "%s", BD->VERSAO_BDADOS);
//    fscanf(ExpBD, "%d", &nt);
//    printf("%s\n", BD->NOME_BDADOS);
//    printf("%s\n", BD->VERSAO_BDADOS);
//    printf("%d\n", nt);
//
//    char NomeTABELA[50];
//    char LerCampos[50];
//    char LerRegistos[50];
//    int nCampos;
//    int nRegistos;
//
//
//    for (int i = 0; i < nt; i++) {
//        fscanf(ExpBD, "%s", NomeTABELA);
//        printf("%s\n", NomeTABELA);
//        TABELA *nomeTb = Criar_Tabela(BD, NomeTABELA);
//        fscanf(ExpBD, "%d", &nCampos);
//        printf("%d\n", nCampos);
//        fscanf(ExpBD, "%s", LerCampos);
//        printf("[%s]\n", LerCampos);
//
//
//        char *tmp = (char *) malloc(sizeof(char) * (strlen(LerCampos) + 1));
//        strcpy(tmp, LerCampos);
//
//        char *data = strtok(tmp, ";");
//        int is_separated = 0;
//        char buffer[50];
//        while (data != NULL) {
//            char *registo = (char *) malloc(sizeof(char) * strlen(data) + 1);
//            strcpy(registo, data);
//            data = strtok(NULL, ";");
//
//            char *fields = strtok(data, "|");
//            printf("----next header\n");
//            while (fields != NULL) {
//                printf("---%s\n", fields);
//                if(!is_separated){
//                    strcpy(buffer,fields);
//                    is_separated = 1;
//                }else{
//                    Add_Campo_Tabela(nomeTb, buffer, fields);
//                    is_separated = 0;
//                }
//                fields = strtok(NULL, " |");
//            }
//
//
//        }
//        tmp = NULL;
//        free(tmp);
//
//
//        fscanf(ExpBD, "%d", &nRegistos);
//        for (int b = 0; b < nRegistos; b++) {
//
//            fscanf(ExpBD, "%s", LerRegistos);
//            Add_Valores_Tabela(nomeTb, LerRegistos);
//            printf("%s\n", LerRegistos);
//
//        }
//    }
//
//    fclose(ExpBD);
//    return SUCESSO;
//}
int Importar_BDados_Excel(BDadosCoupe *BD, char *ficheir_csv) {
    if (!BD) return INSUCESSO;
    if (!ficheir_csv) return INSUCESSO;

    char extension[5] = ".csv";
    char *file_name = NULL;
    char *have_extension = strstr(ficheir_csv, ".csv");
    //Handle file passed with extension or just file name
    if (!have_extension) {
        //Passed file has no extension.We add it for you ?
        printf("Nao tem  extensao\n");
        file_name = (char *) malloc(sizeof(char) * strlen(ficheir_csv) + strlen(extension) + 1);
        strcpy(file_name, ficheir_csv);
        strcat(file_name, extension);
    } else {
        //Passed file has extension. All good to go :)

        file_name = (char *) malloc(sizeof(char) * strlen(ficheir_csv) + 1);
        strcpy(file_name, ficheir_csv);

    }
    FILE *ExpBD;

    ExpBD = fopen(file_name, "r");
    if (ExpBD == NULL) {
        printf("Erro!\n");
        exit(-1);
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
    //char LerTipo[10];
    int nCampos;
    int nRegistos;


    for (int i = 0; i < nt; i++) {
        fscanf(ExpBD, "%s", NomeTABELA);
        TABELA *nomeTb = Criar_Tabela(BD, NomeTABELA);
        fscanf(ExpBD, "%d", &nCampos);
        fscanf(ExpBD, "%s", LerCampos);
        printf("Campos lidos %s\n",LerCampos);


        char *tmp = (char *) malloc(sizeof(char) * (strlen(LerCampos) + 1));
        strcpy(tmp, LerCampos);

        char *data = strtok(tmp, ";");
        printf("Table Begin---------\n");
        while (data != NULL) {
            char *registo = (char *) malloc((strlen(data) + 1));
            strcpy(registo, data);
            data = strtok(NULL, ";");
            printf("--- Header %s ---\n", registo);

            char *fields = strtok(registo, "|");


            printf("----next header\n\n");
            free(registo);

//            int pos = 0;
//            int is_separated = 0;
//            char buffer1[50];
//            char buffer2[50];
//
//            while (fields != NULL) {
//                printf("---------  %d ---- %s\n", pos,fields);
//                pos++;
//                if(!is_separated){
//                    strcpy(buffer1,fields);
//                    is_separated = 1;
//                }else{
//                    strcpy(buffer2,fields);
//                    Add_Campo_Tabela(nomeTb, buffer1, buffer2);
//                    is_separated = 0;
//                }
////
////
////                }
//                fields = strtok(NULL, " |");
//            }
//            pos = 0;


            //Add_Campo_Tabela(nomeTb, LerCampos,LerTipo);
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
    return SUCESSO;
}


int Exportar_BDados_Ficheiro_Binario(BDadosCoupe *BD, char *fich_dat) {
    if (!BD) return -1;
    if (!fich_dat) return -1;

    int i = 0;
    char extension[5] = ".dat";
    char *file_name = NULL;
    char *have_extension = strstr(fich_dat, ".dat");
    //Handle file passed with extension or just file name
    if (!have_extension) {
        //Passed file has no extension.We add it for you :-)
        printf("Nao tem  extensao\n");
        file_name = (char *) malloc(sizeof(char) * strlen(fich_dat) + strlen(extension) + 1);
        strcpy(file_name, fich_dat);
        strcat(file_name, extension);
    } else {
        //Passed file has extension. All good to go :)
        file_name = (char *) malloc(sizeof(char) * strlen(fich_dat) + 1);
        strcpy(file_name, fich_dat);
    }

    FILE *ExpBD;
    ExpBD = fopen(file_name, "wb");

    NOG *n = BD->LTabelas->Inicio;
    fwrite(BD->NOME_BDADOS, 50, 1, ExpBD);
    fwrite(BD->VERSAO_BDADOS, 50, 1, ExpBD);
    fwrite(&BD->LTabelas->NEL, sizeof(int), 1, ExpBD);
    while (n) {
        TABELA *t = (TABELA *) n->Info;
        NOG *Aux = t->LCampos->Inicio;

        fwrite(&t->LCampos->NEL, sizeof(int), 1, ExpBD);
        printf("Exportei n campos : %d\n", t->LCampos->NEL);

        while (Aux) {
            CAMPO *C = (CAMPO *) Aux->Info;
            fwrite(C, sizeof(CAMPO), 1, ExpBD);
            Aux = Aux->Prox;
        }

//            fwrite(t->NOME_TABELA,50,1,ExpBD);
//            fwrite(&t->LCampos->NEL,sizeof(int),1,ExpBD);
//            while (Aux) {
//                char *s = (char *)Aux->Info;
//                int N = strlen(s) + 1;
//                fwrite(&N, sizeof (int), 1, ExpBD);
//                fwrite(s,sizeof(char),N,ExpBD);
//                Aux = Aux->Prox;
//            }
//            Aux = t->LRegistos->Inicio;
//            fwrite(&t->LRegistos->NEL,sizeof(int),1,ExpBD);
//            while (Aux) {
//                ListaGenerica *R = (ListaGenerica *) Aux->Info;
//                NOG *P = R->Inicio;
//                while(P){
//                    char *y = (char *)Aux->Info;
//                    int M = strlen(y) + 1 ;
//                    fwrite(&M, sizeof (int), 1, ExpBD);
//                    fwrite(y,sizeof(char),M,ExpBD);
//                    P = P->Prox;
//                }
//                Aux = Aux->Prox;
//            }
        n = n->Prox;
    }
    fclose(ExpBD);
    free(file_name);
    return SUCESSO;
}

int Importar_BDados_Ficheiro_Binario(BDadosCoupe *BD, char *fich_dat) {

    if (!BD) return INSUCESSO;
    if (!fich_dat) return INSUCESSO;

    char extension[5] = ".dat";
    char *file_name = NULL;
    char *have_extension = strstr(fich_dat, ".dat");
    //Handle file passed with extension or just file name
    if (!have_extension) {
        //Passed file has no extension.We add it for you ?
        printf("Nao tem  extensao\n");
        file_name = (char *) malloc(sizeof(char) * strlen(fich_dat) + strlen(extension) + 1);
        strcpy(file_name, fich_dat);
        strcat(file_name, extension);
    } else {
        //Passed file has extension. All good to go ?

        file_name = (char *) malloc(sizeof(char) * strlen(fich_dat) + 1);
        strcpy(file_name, fich_dat);

    }
    FILE *ExpBD;

    ExpBD = fopen(file_name, "rb");
    if (ExpBD == NULL) {
        printf("Erro!\n");
        exit(-1);
    }
    int nt = 0;

    fread(BD->NOME_BDADOS, 50, 1, ExpBD);
    fread(BD->VERSAO_BDADOS, 50, 1, ExpBD);
    fread(&nt, sizeof(int), 1, ExpBD);
    printf("%s\n", BD->NOME_BDADOS);
    printf("%s\n", BD->VERSAO_BDADOS);
    printf("%d\n", nt);

    char NomeTABELA[50];
    char LerCampos[200];
    char LerRegistos[50];
    int nCampos;
    int nRegistos;
    int nCharCampos;
    int nCharReg;

    for (int ind_t = 0; ind_t < nt; ind_t++) {
        //fread(&nt,sizeof(int),1,ExpBD);
        fread(&nCampos, sizeof(int), 1, ExpBD);
        // printf("Li n Campos- %d\n",nt);
        printf("valor ncampos%d\n", nCampos);
        for (int i = 0; i < nCampos; i++) {
            CAMPO C;
            fread(&C, sizeof(CAMPO), 1, ExpBD);
            printf("Li tamanho campos - %s : %s\n", C.NOME_CAMPO, C.TIPO);
        }
    }

//    for (int i = 0; i < nt; i++) {
//        fread(NomeTABELA,50,1,ExpBD);
//        printf("%s\n", NomeTABELA);
//        TABELA *nomeTb = Criar_Tabela(BD, NomeTABELA);
//        fread( &nCampos,sizeof(int),1,ExpBD);
//        printf("%d\n", nCampos);
//
//    for (int a = 0 ; a < nCampos; a++){
//            fread( &nCharCampos,sizeof(int),1,ExpBD);
//            fread(LerCampos,sizeof(nCharCampos),1,ExpBD);
//            printf("%s\n",LerCampos);
//            printf("%d , %d",a,nCampos);
//       }
//
//       fread( &nRegistos,sizeof(int),1,ExpBD);
//
//        for (int b = 0; b < nRegistos; b++) {
//            fread( &nCharReg,sizeof(int),1,ExpBD);
//            fread(LerRegistos,sizeof(nCharReg),1,ExpBD);
//            printf("%s\n",LerRegistos);
//            printf("%d , %d",b,nRegistos);
//        }
//    }
    fclose(ExpBD);
    return SUCESSO;
}


void destroy_info_string(void *info) {
    //this info is a string pointer
    free(info);
    //Force the info to be NULL - optional but cleaner and safer
    info = NULL;

}

void destroy_info(void *info) {
//    if(info!=NULL){
//        free(info);
//        return;
//    }
    printf("Info is already free\n");
}

//L)	Apagar o conte�do de uma Tabela. A Tabela continua a existir na BDados, mas n�o cont�m os dados, ou seja, os campos continuam, mas os registos s�o eliminados.
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

//M)	Apagar o conte�do de uma Tabela e remove a tabela da base de dados.
int DROP_TABLE(BDadosCoupe *BD, char *nome_tabela) {
    if (!BD)return INSUCESSO;
    if (strlen(nome_tabela) < 1)return INSUCESSO;
    TABELA *t = Pesquisar_Tabela(BD, nome_tabela);
    if (t) {
        printf("Droping table %s\n", t->NOME_TABELA);
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

//        NOG *actual,*prev = BD->LTabelas->Inicio;
//        if (actual != NULL && actual->Info == t) {
//            //actual = actual->Prox; // Changed head
//            free(actual->Info); // free old head
//           // free(t); // free old head
//            BD->LTabelas --;
//            prev = actual->Prox;
//            return SUCESSO;
//        }



//        if (t->LCampos->NEL > 0) {
//            while (t->LCampos->Inicio) {
//                printf("Removendo campo %s\n", t->LCampos->Inicio->Info);
//                NOG *node = t->LCampos->Inicio;
//
//                free((CAMPO *) node->Info);
//
//
//                t->LCampos->Inicio = node->Prox;
//                free(node);
//                t->LCampos->NEL--;
//            }
//            printf("Lista campos %d\n", t->LCampos->NEL);
//            //todo - perceber pk este free da erro!
//            // printf("Lista campos %",t->LCampos->Inicio);
//            free(t->LCampos);
//
//        } else {
//            free(t->LCampos);
//        }
//        printf("elementos %d:\n", BD->LTabelas->NEL);
//        //Remover tabela da lista Tabelas
//        if (BD->LTabelas->NEL <= 1) {
//            //Verificar se o unico elemento e o que procuramos
//            NOG *n = BD->LTabelas->Inicio;
//            printf("Tentando remover unico elemento\n");
//            TABELA *aux = BD->LTabelas->Inicio->Info;
//            if (strcmp(aux->NOME_TABELA, t->NOME_TABELA) == 0) {
//                printf("Removendo unico elemento\n");
//                free(aux);
//                free(n);
//                BD->LTabelas->NEL--;
//                return SUCESSO;
//            }
//        } else {
//            NOG *actual = BD->LTabelas->Inicio;
//            while (actual) {
//                NOG *next = actual->Prox;
//                TABELA *atualT = (TABELA *) actual->Info;
//
//
//                if (strcmp(atualT->NOME_TABELA, t->NOME_TABELA) == 0) {
//                    printf("Found %s\n", atualT->NOME_TABELA);
//
//                    //Nao existe um proximo
//                    if (next == NULL) {
//                        printf("matar");
//                        free(actual);
//                        BD->LTabelas->NEL--;
//                        return SUCESSO;
//                        //actual
//                    } else {
//                        actual->Prox = next->Prox;
//                        free(actual);
//                        BD->LTabelas->NEL--;
//                        return SUCESSO;
//                    }
//
//                }
//                actual = actual->Prox;
//            }
//            return INSUCESSO;
//        }
        return SUCESSO;
    }
    return INSUCESSO;
}

void printResultNode(NOG *node) {
//Iterate all the nodes and print the info
    NOG *p = ((REGISTO *) (node->Info))->Inicio;
    while (p) {
        printf("%s\t\t", (char *) p->Info);
        p = p->Prox;
    }
    printf("\n");
    //printf("+------------------------------+\n");
}

void showHeaderTable(NOG *node, char *title) {
    printf("+------------------ %s  QUERY---------------+\n", title);
    int formatter = 0;
    while (node) {
        if (formatter > 0) {
            printf("[%s]\t\t", (char *) node->Info);
            formatter = 0;
        } else {
            printf("%s", (char *) node->Info);
            formatter++;
        }

        node = node->Prox;
    }
    printf("\n");

}

//N)	Selecionar (Apresentar no ecran!) da base de dados todos os registos que obede�am a uma dada condi��o, a fun��o deve retornar o n�mero de registos selecionados. (Ter em aten��o o exemplo das aulas te�ricas!). Nota: esta � certamente a funcionalidade mais usada num sistema de base de dados�, por isso se estiver bem otimizada�. O utilizador agradece!!!!
int
SELECT(BDadosCoupe *BD, char *_tabela, int (*f_condicao)(char *, char *), char *nome_campo, char *valor_comparacao) {
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
    return SUCESSO;
}


//NOG *deleteRegister(ListaGenerica *l, NOG *currentNode) {
//
//    if (!l) return NULL;
//    if (!currentNode) return NULL;
//    NOG *node = l->Inicio;
//    NOG *aux = NULL;
//    while (node) {
//        if (node == currentNode) {
//            printf("encontrei o node %s", node->Info);
//            aux = node->Prox;
//            DestruirLG(currentNode->Info, destroy_info_string);
//            free(node);
//            l->NEL--;
//            l->Inicio = aux;
//        }
//        node = node->Prox;
//
//        printf("aqui");
//
//    }
//
//
//}

NOG *removeRegister(ListaGenerica *l, NOG *element) {
    if (!l)return NULL;
    int procura = 1;
    NOG *ret, *ant, *actual;
    ant = actual = l->Inicio;
    while (procura) {
        if (actual == NULL)procura = 0;
        else if (actual == element)
            procura = 0;
        else {
            ant = actual;
            actual = actual->Prox;
        }
        if (actual != NULL) {
            ret = actual;
            if (l->Inicio == actual) {
                DestruirLG(actual->Info, destroy_info_string);
                l->NEL--;
                l->Inicio = actual->Prox;
            } else {
                DestruirLG(actual->Info, destroy_info_string);
                l->NEL--;
                ant->Prox = actual->Prox;
            }
        } else {
            ret = NULL;
        }
        return ret;
    }

    return NULL;
}

//O)	Remover todos os registos que obede�am a uma dada condi��o, a fun��o deve retornar o n�mero de registos removidos.
int
DELETE(BDadosCoupe *BD, char *_tabela, int (*f_condicao)(char *, char *), char *nome_campo, char *valor_comparacao) {
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
        showHeaderTable(table->LCampos->Inicio, "DROP");
        int register_count = 0;
        //Pointer to the REGISTOS List
        NOG *node = table->LRegistos->Inicio;
        while (node) {
            NOG *p = ((REGISTO *) (node->Info))->Inicio;
            int pos = 0;
            //NOG*aux  = NULL;
            while (p) {
                if (field_pos == pos) {
                    printf("Comparing %s with %s", (char *) p->Info, valor_comparacao);
                    if (f_condicao((char *) p->Info, valor_comparacao)) {
                        printResultNode(removeRegister(table->LRegistos, node));
                    }
                }
                pos++;
                p = p->Prox;
            }
            node = node->Prox;
        }
        printf("+------------------------------+\n");
        printf("Foram encontrados %d registos\n", register_count);

    }
    return SUCESSO;
}

void updateREGISTER(NOG *r, int field_pos, char *newValue) {
    if (!r)return;
    if (!newValue)return;
    int pos = 0;
    while (r) {
        if (field_pos = pos) {
            printf("Atualizando ID %s na posicao %d com valor novo de %s\n", (char *) r->Info, field_pos, newValue);
            free(r->Info);
            r->Info = (char *) malloc(sizeof(char) * strlen(newValue) + 1);
            strcpy(r->Info, newValue);
            return;
        }
        pos++;
        r = r->Prox;
    }


}

//P)	Atualizar todos os registos da tabela onde o Campo � dado, que obede�am a uma dada condi��o, a fun��o deve retornar o n�mero de registos que foram atualizados.
int UPDATE(BDadosCoupe *BD, char *_tabela, int (*f_condicao)(char *, char *), char *campo_comp, char *valor_campo_comp,
           char *nome_campo_update, char *valor_campo_update) {
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
        int field_pos;
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
        printf("Campo no indice %d\n\n", field_pos);

        showHeaderTable(table->LCampos->Inicio, "UPDATE");
        int register_count = 0;
        //Pointer to the REGISTOS List
        NOG *node = table->LRegistos->Inicio;
        while (node) {
            NOG *p = ((REGISTO *) (node->Info))->Inicio;
            int pos = 0;
            while (p) {
                if (field_pos == pos) {
                    //printf("Localizando Registo %s VS %s\n",(char *) p->Info,valor_campo_comp);
                    if (f_condicao((char *) p->Info, valor_campo_comp)) {
                        //printf("Encontrei o registo. vamos alterar ...\n");
                        updateREGISTER(p, field_update_pos, valor_campo_update);
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
        printf("Foram encontrados %d registos\n", register_count);

    } else {
        printf("Tabela nao encontrada!!!\n");
    }
    return SUCESSO;
}



