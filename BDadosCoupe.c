
#include <stdint.h>
#include <ctype.h>

#include "BDadosCoupe.h"

#define FIXED_SIZE_ARRAY 50L

/** \brief Criar_BDados: A) Criar a Base de dados
 *
 * \param nome_bd char* : Nome da Base de Dados
 * \param versao char*  : Versão da Base de Dados
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
 * a tabela deve ser inserida à lista de tabelas da BD
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

//D)	Adicionar dados(registos) a uma tabela, os dados são dados numa string onde o separador é ‘;’m ex: Add_Valores_Tabela(T, “123;Joao;965654449”)
int Add_Valores_Tabela(TABELA *T, char *dados) {
    if (!T)return INSUCESSO;
    if (dados) {

        char *tmp = (char *) malloc(sizeof(strlen(dados) + 1));
        strcpy(tmp, dados);
        char *data = strtok(tmp, ";");
        while (data != NULL) {
            char *registo = (char *) malloc(sizeof(strlen(data) + 1));
            strcpy(registo, data);
            data = strtok(NULL, ";");
            AddLG(T->LRegistos, registo);
        }
        //todo resolver este free
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
    printf("------------Tabela [%s]------------\n", T->NOME_TABELA);
    NOG *n = T->LCampos->Inicio;
    int fields_count = T->LCampos->NEL;
    int pos = 0;
    while (n) {
        CAMPO *c = (CAMPO *) n->Info;
        printf("[%s] (%s)\t", c->NOME_CAMPO, c->TIPO);
        n = n->Prox;
    }
    printf("\n");
    n = T->LRegistos->Inicio;
    while (n) {
        if (pos >= fields_count) {
            printf("\n");
            pos = 0;
        }
        printf("%s\t\t", (char *) n->Info);
        pos++;
        n = n->Prox;

    }
    printf("\n-----[A tabela contem %d registos]-----\n\n", T->LRegistos->NEL / T->LCampos->NEL);
}

//H)	Mostrar toda a base de dados, deverá mostrar todas as Tabelas da BDados.
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

//I)	Libertar toda a memória alocada pela base de dados.
void Destruir_BDados(BDadosCoupe *BD) {
    if (!BD)return;
    if (BD->LTabelas->NEL > 0) {
        printf("%d\n", BD->LTabelas->NEL);
        while ( BD->LTabelas->NEL) {
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

//J)	Memória ocupada por toda a base de dados.
long int Memoria_BDados(BDadosCoupe *BD) {
    return -1;
}

long int Memoria_Desperdicada_BDados(BDadosCoupe *BD) {
    if (!BD) return -1;
    long wasted_memory = 0;
    //Evaluate the memory used in BD head
    wasted_memory += FIXED_SIZE_ARRAY - (strlen(BD->NOME_BDADOS) + 1);
    wasted_memory += FIXED_SIZE_ARRAY - (strlen(BD->VERSAO_BDADOS) + 1);
    //Evaluate the memory in each ListTabelas - nome_tabela
    ListaGenerica *head = (ListaGenerica *) BD->LTabelas;
    //printf("A lista contem %d campos\n", head->NEL);
    NOG *node = head->Inicio;
    while (node) {
        TABELA *t = (TABELA *) node->Info;
        wasted_memory += FIXED_SIZE_ARRAY - (strlen(t->NOME_TABELA) + 1);
        NOG *node_LCampos = t->LCampos->Inicio;
        while (node_LCampos) {
            //printf("looping node campo\n");
            CAMPO *c = (CAMPO *) node_LCampos->Info;
            wasted_memory += FIXED_SIZE_ARRAY - (strlen(c->TIPO) + 1);
            //printf("Memoria do tipo da campo %s %d\t\t\n", c->TIPO, strlen(c->TIPO) + 1);
            wasted_memory += FIXED_SIZE_ARRAY - (strlen(c->NOME_CAMPO) + 1);
            //printf("Memoria do nome da campo %s %d\t\t\n", c->NOME_CAMPO, strlen(c->NOME_CAMPO) + 1);
            node_LCampos = node_LCampos->Prox;
        }
        node = node->Prox;
    }

    printf("Memoria desperdicada na BD %ld BYTES \n", wasted_memory);
    return wasted_memory;

    //Analisar desperdicio nos seguintes campos
//    BD
//      char NOME_BDADOS[50];
//      char VERSAO_BDADOS[50];
//  TABELA
//    char NOME_TABELA[50];
//  CAMPO
//    char NOME_CAMPO[50];
//    char TIPO[10];
//  REGISTO


}

//K)	Exportar/Importar para/de Ficheiro (o retorno destas funções, permite saber se a função foi bem/mal-executada!):
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
                fprintf(ExpBD, "%s;", (char *) Aux->Info);
                Aux = Aux->Prox;
            }
            fprintf(ExpBD, "\n");
            Aux = t->LRegistos->Inicio;
            fprintf(ExpBD, "%d\n", t->LRegistos->NEL / t->LCampos->NEL);
            while (Aux) {
                fprintf(ExpBD, "%s;", (char *) Aux->Info);
                Aux = Aux->Prox;
                i++;
                if (i == t->LCampos->NEL) {
                    fprintf(ExpBD, "\n");
                    i = 0;
                }
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

int Importar_BDados_Excel(BDadosCoupe *BD, char *ficheir_csv) {
    if (!BD) return INSUCESSO;
    if (!ficheir_csv) return INSUCESSO;

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

    ExpBD = fopen(file_name, "r");
    if (ExpBD) {
        char teste[50];
        char Buffer[20];
        while (!feof(ExpBD)) {
            fscanf(ExpBD, "%s\n", Buffer);

            //printf("%s\n",Buffer);
            fgets(teste, 100, ExpBD);

            printf("%s", teste);


        }
        fclose(ExpBD);
        return SUCESSO;

    }
    printf("Erro ao recriar BD via ficheiro!\n");
    exit(-1);


}

int Exportar_BDados_Ficheiro_Binario(BDadosCoupe *BD, char *fich_dat) {
    return SUCESSO;
}

int Importar_BDados_Ficheiro_Binario(BDadosCoupe *BD, char *fich_dat) {
    return SUCESSO;
}

//L)	Apagar o conteúdo de uma Tabela. A Tabela continua a existir na BDados, mas não contém os dados, ou seja, os campos continuam, mas os registos são eliminados.
int DELETE_TABLE_DATA(TABELA *T) {
    if (!T)return INSUCESSO;
    if (T->LRegistos->NEL > 0) {
        NOG *node = T->LRegistos->Inicio;
        while (T->LRegistos->NEL) {

            NOG *aux = node;
            free(aux->Info);
            node = aux->Prox;
            free(aux);
            T->LRegistos->NEL--;
        }
        return SUCESSO;
    }
    return INSUCESSO;
}

int DELETE_TABLE_FIELDS(TABELA *T) {
    if (!T)return INSUCESSO;
    if (T->LCampos->NEL > 0) {
        NOG *node = T->LCampos->Inicio;
        while (node) {
            //printf("Removendo campos %s\n", node->Info);
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

//M)	Apagar o conteúdo de uma Tabela e remove a tabela da base de dados.
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
            printf("aqui");
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

//N)	Selecionar (Apresentar no ecran!) da base de dados todos os registos que obedeçam a uma dada condição, a função deve retornar o número de registos selecionados. (Ter em atenção o exemplo das aulas teóricas!). Nota: esta é certamente a funcionalidade mais usada num sistema de base de dados…, por isso se estiver bem otimizada…. O utilizador agradece!!!!
int
SELECT(BDadosCoupe *BD, char *_tabela, int (*f_condicao)(char *, char *), char *nome_campo, char *valor_comparacao) {
    return SUCESSO;
}

//O)	Remover todos os registos que obedeçam a uma dada condição, a função deve retornar o número de registos removidos.
int
DELETE(BDadosCoupe *BD, char *_tabela, int (*f_condicao)(char *, char *), char *nome_campo, char *valor_comparacao) {
    return SUCESSO;
}

//P)	Atualizar todos os registos da tabela onde o Campo é dado, que obedeçam a uma dada condição, a função deve retornar o número de registos que foram atualizados.
int UPDATE(BDadosCoupe *BD, char *_tabela, int (*f_condicao)(char *, char *), char *campo_comp, char *valor_campo_comp,
           char *nome_campo_update, char *valor_campo_update) {
    return SUCESSO;
}

