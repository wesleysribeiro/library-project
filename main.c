#include <stdio.h>
#include <string.h> /* Para usar strstr */
#include <ctype.h> /* Para usar tolower */
#include <stdlib.h> /* Usar funcao exit, atoi */

#define SIZE 200
#define ARQUIVO "Datebase.txt"
#define TEMP "Temp.txt"

/* Declaração dos arquivos */
FILE *fp;


typedef struct{
    char name[SIZE];
    char author[SIZE];
    int year;
    int npages;
    unsigned int location;
    char status;
}Book_Info;

void OpenFiles(){
    fp = fopen(ARQUIVO, "r+b");
    if(fp == NULL)
        fp = fopen(ARQUIVO, "w+b");
    if(fp == NULL){
        fprintf(stdout, "ERRO: Impossivel criar arquivo\n");
        getchar();
        exit(1);
    }
}

void show_book(Book_Info a){
	putchar('\n');
    puts("-----------------------------------");
    printf("Nome: %s\n", a.name);
    printf("Autor: %s\n", a.author);
    printf("Ano: %d\n", a.year);
    printf("Num de paginas: %d\n", a.npages);
    printf("Localizacao: %d\n", a.location);
    printf("Status: %s\n", a.status == '1'? "Disponivel": "Emprestado");
}

void insert_book(){
    Book_Info b;
    char c;
    if(fgetc(fp)!=EOF) {
        fseek(fp, -(long) sizeof(b), SEEK_END);
        fread(&b, sizeof(b), 1, fp);
        b.location+=1;
    }
    else
        b.location = 1;
    b.status = '1';
    printf("Informe o nome do livro: ");
    gets(b.name);
    printf("Informe o nome do autor: ");
    gets(b.author);
    printf("Informe o numero de paginas: ");
    scanf("%d", &b.npages);
    printf("Informe o ano do livro: ");
    scanf("%d", &b.year);
    fseek(fp, 0L, SEEK_END);
    fwrite(&b, sizeof(b), 1, fp);
}

/* Se num = 1, busca por titulo, se num = 2, busca por ano de lancamento, para num = 3 busca por autor */
void search_book(char *str, int num){
    Book_Info b;
    if (num == 1)
		while ((fread(&b, sizeof(b), 1, fp)) == 1){
            if (strstr(b.name, str) != NULL)
                show_book(b);
            }
    else
            if(num == 2)
                while ((fread(&b, sizeof(b), 1, fp)) == 1) {
                    if (b.year == atoi(str))
                        show_book(b);
                }
            else
                while ((fread(&b, sizeof(b), 1, fp)) == 1) {
                    if (strstr(b.author, str))
                        show_book(b);
                }
}

void remove_book(unsigned int n_location){
    Book_Info b, a;
    char c;
    while (fread(&b, sizeof(b), 1, fp) == 1)
    	if(b.location == n_location){	
    		fseek(fp, sizeof(b), SEEK_CUR);
    		fread(&a, sizeof(a), 1, fp);
    	}
}


/* Retorna 1 se foi possivel modificar, caso contrario retorna -1 */
int modify_register(long int n_location){
	int mode;
	char is_valid_location = '0';
    Book_Info b;
	while(fread(&b, sizeof(b), 1, fp) == 1)
		if(b.location == n_location){
			show_book(b);
			puts("-----------------------------------");
			printf("1 -\t Modificar titulo\n");
        	printf("2 -\t Modificar ano\n");
        	printf("3 -\t Modificar autor\n");
        	printf("4 -\t Modificar numero de paginas\n");
        	printf("5 -\t Modificar status do livro\n");
        	puts("-----------------------------------");
        	printf("Informe o item que deseja modificar: ");
        	scanf("%d", &mode);
        	fflush(stdin);
			switch(mode){
				case 1:
					printf("Digite o novo nome do titulo: ");
					gets(b.name);
					is_valid_location = '1';
					break;
				case 2:
					printf("Digite o novo ano do livro: ");
					scanf("%d", &b.year);
					is_valid_location = '1';
					break;
				case 3:
					printf("Digite o novo nome do autor: ");
					gets(b.author);
					is_valid_location = '1';
					break;
				case 4:
					printf("Digite o novo numero de paginas: ");
					scanf("%d", &b.npages);
					is_valid_location = '1';
					break;
				case 5:
					printf("Digite o novo status (1 - Disponivel) (0 - Emprestado): ");
					b.status = getchar();
					is_valid_location = '1';
					break;
				default:
					printf("ERRO: Modificacao invalida!\a\n");
					return -1;
			}
			break;
		}
	if(is_valid_location == '1'){
		fseek(fp, -(long) sizeof(b), SEEK_CUR);
		if(fwrite(&b, sizeof(b), 1, fp) == 1)
			printf("Modificacao realiza com sucesso!\n");
		return 1;
	}
	else{
		printf("Localizacao invalida!!!\a\n");
		return -1;
	}
}

void show_all_books(){
    Book_Info b;
    while(1){
        if(fread(&b, sizeof(b), 1, fp)!=1){
            break;
        }
        show_book(b);
    }
}


int main(){
    int option, search_option;
	unsigned int n_location;
    char name[SIZE];
    OpenFiles();
    printf("1 -\t Adicionar livro ao registro\n");
    printf("2 -\t Buscar livro\n");
    printf("3 -\t Remover livro do registro\n");
    printf("4 -\t Alterar registro\n");
    printf("5 -\t Mostrar acervo\n");
    printf("0 -\t Sair\n");
    printf("Informe a opcao desejada: ");
    scanf("%d", &option);
    fflush(stdin);
    switch(option){
        case 1:
            insert_book();
            break;
        case 2:
            printf("1 - Procurar por nome\n");
            printf("2 - Procurar por data\n");
            printf("3 - Procurar por autor\n");
            printf("Digite a opcao desejada: ");
            scanf("%d", &search_option);
            fflush(stdin);
            switch(search_option){
                case 1:
                case 2:
                case 3:
                    printf("Informe o dado de busca: ");
                    gets(name);
                    search_book(name, search_option);
                    break;
                default:
                    printf("Opcao invalida !!\a\a");
            }
            break;
        case 3:
        	printf("Digite a localizacao do livro: ");
        	scanf("%d", &n_location);
            remove_book(n_location);
            break;
        case 4:
        	printf("Digite a localizacao do livro: ");
        	scanf("%d", &n_location);        	
			fflush(stdin);
            modify_register(n_location);
            break;
        case 5:
            show_all_books();
        case 0:
            break;
        default:
            printf("Digite um valor valido!!!\a\a");
    }
    getchar();
    fclose(fp);
    return 0;
}
