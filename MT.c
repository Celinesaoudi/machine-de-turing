#include "stdio.h"
#include "stdlib.h"
#include "string.h"




//Enum pour déplacement
enum Deplace {
    Left,
    Right,
    Static,

};
//Structure pour Transition
struct Transition {
    char cur_state[3];
    char entry_read;
    char next_state[3];
    char entry_write;
    enum Deplace depl;
};

//Structure qui décrit une MT
struct MT {
    char* state[100]; //remplie
    struct Transition* T; //remplie
    char band[10000]; //remplie
    char cur_state[3]; //remplie
    int read_position; 
    int nbr_etat; //rempli
    int nbr_transition; //rempli
};


/*
void print_MT(struct MT* M_turing) {
    printf("nombre d'états: %d\n",M_turing->nbr_etat);
    for (int i =0; i < M_turing->nbr_etat; i++) {
        printf ("l'état %d: %c\n",i,M_turing->state[i]);
    }
    printf("les transition :\n");
    for (int i = 0; i < M_turing->nbr_transition; i++){
        printf("transition %d: \n",i);
        printf("cur_state: %c\n",M_turing->T[i].cur_state);
        printf("entry_read: %c\n",M_turing->T[i].entry_read);
        printf("next_state: %c\n",M_turing->T[i].next_state);
        printf("entry_write: %c\n",M_turing->T[i].entry_write);
         switch (M_turing->T[i].depl) {
            case Left: printf("depl: <\n"); break;
            case Right: printf("depl: >\n"); break; 
            case Static: printf("depl: -\n"); break;
            default: printf ("error: Deplacement\n"); 
            exit(EXIT_FAILURE);
            break;
        }



}
}*/
int is_in(struct MT* M_turing,char* c) {
    if (M_turing->nbr_etat == 0) { return 0;}
    for (int i = 0; i < M_turing->nbr_etat; i++) {
        if (!strcmp(M_turing->state[i], c)) {return 1;}
    }
    return 0;
}


//Fonction pour l'initialisation d'une MT
struct MT* init_MT(char* path) {
    struct MT* M_turing = malloc(sizeof(struct MT));
    int count_transition = 0;
    int c=0;
    char String[15];

    FILE* file = fopen(path,"r");

    //Compte le nombre de transition
    while (c != EOF) {
        fscanf(file, "%[^\n]",String);
        c=fgetc(file);
        count_transition++;
    }
    count_transition = count_transition/2;
    M_turing->nbr_transition=count_transition;

    M_turing->T = malloc(sizeof(struct Transition)*count_transition);
    rewind(file);
    M_turing->nbr_etat=0;
    for (int i = 0; i < count_transition; i++){



        fscanf(file, "%[^,]",String);

        
        if (!is_in(M_turing, String)) {
                M_turing->state[M_turing->nbr_etat]= malloc(sizeof(char)*4);
               strcpy(M_turing->state[M_turing->nbr_etat], String);
                M_turing->nbr_etat++;
        }

        strcpy(M_turing->T[i].cur_state, String);
        fgetc(file);
        fscanf(file, "%[^\n]",String);
        if (String[0] == '_') {M_turing->T[i].entry_read='_';}
        else M_turing->T[i].entry_read = String[0];
        fgetc(file);



        fscanf(file, "%[^,]",String);
        
        if (!is_in(M_turing, String)) {
                  M_turing->state[M_turing->nbr_etat]= malloc(sizeof(char)*4);
               strcpy( M_turing->state[M_turing->nbr_etat], String);
                M_turing->nbr_etat++;
        }
                
 
        strcpy (M_turing->T[i].next_state, String);
        fgetc(file);
         fscanf(file, "%[^\n]",String);
         if (String[0] == '_') {M_turing->T[i].entry_write='_';}
        else M_turing->T[i].entry_write = String[0];
        switch (String[2]) {
            case '<': M_turing->T[i].depl = Left; break;
            case '>': M_turing->T[i].depl = Right; break; 
            case '-': M_turing->T[i].depl = Static; break;
            default: printf ("error: Deplacement\n"); 
            exit(EXIT_FAILURE);
            break;
        }
        fgetc(file);

    }

       strcpy( M_turing->cur_state, M_turing->state[0]);
        M_turing->read_position = 1;





    return M_turing;

}


int first_step(struct MT* M_turing) {
            
        for (int i = 0; i < M_turing->nbr_transition; i++) { 
            if (!strcmp(M_turing->cur_state, M_turing->T[i].cur_state) ) { 
              
                if (M_turing->band[M_turing->read_position] == M_turing->T[i].entry_read) {
                    strcpy (M_turing->cur_state, M_turing->T[i].next_state);
                    M_turing->band[M_turing->read_position] = M_turing->T[i].entry_write;
                    switch (M_turing->T[i].depl) {
                        case Left: M_turing->read_position--; 
                            break;
                        case Right: M_turing->read_position++;
                            break;
                        case Static: break;

                    }

                    return 1;
                }
            }
        }

        return 0;
}
     

int MT_Simulator(char* entry, struct MT* M_turing) {
        int Accept = 1;
        strcpy(&M_turing->band[1], entry);
        for (int i = 0; i < 10000; i++) {
             if (M_turing->band[i] != '0' && M_turing->band[i] != '1') {M_turing->band[i] = '_';}
          
        }

            while (strcmp(M_turing->cur_state, "F") && Accept == 1) {
                
                Accept = first_step(M_turing);
                
            }

    return Accept;
}





void to_binary_MT(char* path) {
char String[15];
char entry_binary[4];
char cur_state;
char entry_read;
char entry_write;
char next_state;
char depl;
char c;
int count_transition = 0;
 FILE* file = fopen(path,"r");
 FILE* file_write = fopen("MT_TO_BINARY.txt","w");

while (c != EOF) {
        fscanf(file, "%[^\n]",String);
        c=fgetc(file);
        count_transition++;
    }
    count_transition = count_transition/2;
    rewind(file);

    for (int i = 0; i < count_transition; i++) {
 fscanf(file, "%[^\n]",String);
 cur_state=String[0];
 entry_read=String[2];
 fgetc(file);
 fscanf(file, "%[^\n]",String);
 next_state=String[0];
 entry_write=String[2];
 depl=String[4];
fgetc(file);

    switch (entry_read) {
        case 'a' : strcpy (entry_binary, "00"); break; 
        case 'b' : strcpy (entry_binary, "01"); break; 
        case 'c' : strcpy (entry_binary, "10"); break; 
        case 'd' : strcpy (entry_binary, "11"); break; 
        case '_' : strcpy (entry_binary, "__"); break; 
    }
    
 fprintf(file_write, "%c,%c\n%c%c,%c,>\n", cur_state, entry_binary[0],  cur_state, entry_binary[0], entry_binary[0]);
    
    
    if (entry_write == 'a') {
        if (depl == '>'){
        fprintf(file_write, "%c%c,%c\n%ca,0,<\n",cur_state,entry_binary[0],entry_binary[1], next_state);
        fprintf(file_write, "%ca,0\n%cr,0,>\n",next_state, next_state);
        fprintf(file_write, "%ca,1\n%cr,0,>\n", next_state, next_state);
        fprintf(file_write, "%cr,0\n%c,0,>\n", next_state, next_state);
        fprintf(file_write, "%cr,1\n%c,1,>\n", next_state, next_state);
        }

        else {
        fprintf(file_write, "%c%c,%c\n%ca,%c,<\n",cur_state,entry_binary[0],entry_binary[1], next_state,entry_binary[1]);
        fprintf(file_write, "%ca,0\n%c,0,%c\n", next_state, next_state, depl);
        fprintf(file_write, "%ca,1\n%c,0,%c\n", next_state, next_state, depl);
        }

        
    }

    else if (entry_write == 'b') {
           if (depl == '>'){
        fprintf(file_write, "%c%c,%c\n%cb,1,<\n",cur_state,entry_binary[0],entry_binary[1], next_state);
        fprintf(file_write, "%cb,0\n%cr,0,>\n",next_state, next_state);
        fprintf(file_write, "%cb,1\n%cr,0,>\n", next_state, next_state);
        fprintf(file_write, "%cr,0\n%c,0,>\n", next_state, next_state);
        fprintf(file_write, "%cr,1\n%c,1,>\n", next_state, next_state);
        }

        else {
        fprintf(file_write, "%c%c,%c\n%cb,1,<\n",cur_state,entry_binary[0],entry_binary[1], next_state);
        fprintf(file_write, "%cb,0\n%c,0,%c\n", next_state, next_state, depl);
        fprintf(file_write, "%cb,1\n%c,0,%c\n", next_state, next_state, depl);
        }

        
    }


    else if (entry_write == 'c') {
           if (depl == '>'){
        fprintf(file_write, "%c%c,%c\n%cc,0,<\n",cur_state,entry_binary[0],entry_binary[1], next_state);
        fprintf(file_write, "%cc,0\n%cr,1,>\n",next_state, next_state);
        fprintf(file_write, "%cc,1\n%cr,1,>\n", next_state, next_state);
        fprintf(file_write, "%cr,0\n%c,0,>\n", next_state, next_state);
        fprintf(file_write, "%cr,1\n%c,1,>\n", next_state, next_state);
        }

        else {
        fprintf(file_write, "%c%c,%c\n%cc,0,<\n",cur_state,entry_binary[0],entry_binary[1], next_state);
        fprintf(file_write, "%cc,0\n%c,1,%c\n", next_state, next_state, depl);
        fprintf(file_write, "%cc,1\n%c,1,%c\n", next_state, next_state, depl);
        }

        
    }

    else if (entry_write == 'd') {
           if (depl == '>'){
        fprintf(file_write, "%c%c,%c\n%cd,1,<\n",cur_state,entry_binary[0],entry_binary[1], next_state);
        fprintf(file_write, "%cd,0\n%cr,1,>\n",next_state, next_state);
        fprintf(file_write, "%cd,1\n%cr,1,>\n", next_state, next_state);
        fprintf(file_write, "%cr,0\n%c,0,>\n", next_state, next_state);
        fprintf(file_write, "%cr,1\n%c,1,>\n", next_state, next_state);
        }

        else {
        fprintf(file_write, "%c%c,%c\n%cd,1,<\n",cur_state,entry_binary[0],entry_binary[1], next_state);
        fprintf(file_write, "%cd,0\n%c,1,%c\n", next_state, next_state, depl);
        fprintf(file_write, "%cd,1\n%c,1,%c\n", next_state, next_state, depl);
        }

        
    }

    else if (entry_write == '_') {
           if (depl == '>'){
        fprintf(file_write, "%c%c,%c\n%c_,_,<\n",cur_state,entry_binary[0],entry_binary[1], next_state);
        fprintf(file_write, "%c_,0\n%cr,_,>\n",next_state, next_state);
        fprintf(file_write, "%c_,1\n%cr,_,>\n", next_state, next_state);
        fprintf(file_write, "%cr,0\n%c,_,>\n", next_state, next_state);
        fprintf(file_write, "%cr,1\n%c,_,>\n", next_state, next_state);
        }

        else {
        fprintf(file_write, "%c%c,%c\n%c_,_,<\n",cur_state,entry_binary[0],entry_binary[1], next_state);
        fprintf(file_write, "%c_,0\n%c,_,%c\n", next_state, next_state, depl);
        fprintf(file_write, "%c_,1\n%c,_,%c\n", next_state, next_state, depl);
        fprintf(file_write, "%c_,_\n%c,_,%c\n", next_state, next_state, depl);
        }

        
    }
    


    }

}






    /*

    void to_mono_infini( char* path) {
        char String[15];
        char Lu[15];
        int count_transition=0;
        FILE* file = fopen(path,"r");
        FILE* file_write = fopen("MT_TO_MONO.txt","w");
        char c;
        char next_state[3];

        while (c != EOF) {
        fscanf(file, "%[^\n]",String);

        c=fgetc(file);
        count_transition++;
        }
        count_transition = count_transition/2;
        rewind(file);
        
        for (int i = 0; i < count_transition; i++) {
        fscanf(file, "%[^,]",String);
        
        fprintf(file_write, "%s,#\n%s1,#,>\n", String, String);
        fprintf(file_write, "%s1,0\n%s1,0,>\n", String, String);
        fprintf(file_write, "%s1,1\n%s1,1,>\n", String, String);
        fprintf(file_write, "%s1,_\n%s2,_,<\n", String, String);
        fprintf(file_write, "%s2,0\n%s3,_,>\n", String, String);
        fprintf(file_write, "%s2,1\n%s4,_,>\n", String, String);
        fprintf(file_write, "%s3,_\n%sr,0,<\n", String, String);
        fprintf(file_write, "%s4,_\n%sr,1,<\n", String, String);
        fprintf(file_write, "%sr,_\n%s2,_,<\n", String, String);
        
        fgetc(file);
        fscanf(file, "%[^\n]",Lu);
        fgetc(file);
        fscanf(file, "%[^,]",next_state);
        fgetc(file);

        fprintf(file_write, "%s2,#\n%s,#,>\n", String, next_state);
       
        fscanf(file, "%[^\n]",Lu);
        fgetc(file);
       
        



    }

    rewind(file); 

    while (c != EOF)
        {

            fscanf(file,"%c",&c);
            fprintf(file_write,"%c",c);
        }




 


} */


int main (int argc, char* argv[]) {
    printf("%d\n", argc);
    if (argc != 3) { printf("\n\033[0;33mLe programme fonction de cette façon\n \033[0m\n\n");
    printf("Tapez une commande : make Entry FilePath\n");
    printf("Entry: L'Entrée\n");
    printf("FilePath: Le chemin du fichier contenant le code MT");
    exit (EXIT_FAILURE);
    }

    int i=1;




    struct MT* M_turing = init_MT(argv[2]);
    
    if (MT_Simulator(argv[1], M_turing)) { printf("Accepted.\n");}
    else printf ("Rejected.\n");
    printf("%c", M_turing->band[0]);
    while (M_turing->band[i] != '_') {
        printf("%c", M_turing->band[i]);
        i++;
    }
        printf("\n");

    



   

    to_binary_MT("MT_change.txt");
   // to_mono_infini("MT_ADD_ZERO.txt");


    return 0;
}