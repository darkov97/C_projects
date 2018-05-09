#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <curses.h>
#include <time.h>



char* read_file(char* fname)
{
	FILE * f;
	size_t len,pos;
	char c;
	char* out;
	f = fopen(fname, "rt"); // otvarame subor v textovom rezime
	if (f == NULL) // kontrolujeme, ci otvaranie prebehlo OK
		return NULL;

	pos = ftell(f);    // aktualna pozicia
    fseek(f, 0, SEEK_END);    // presunieme sa na koniec suboru
    len = ftell(f); // zistenie aktualnej pozicie v subore
    fseek(f, pos, SEEK_SET);  // vratenie sa na zaciatok suboru

	out = malloc(len+1); // alokujeme pamat pre retazec + 1 ukoncovaci znak
	if(out == NULL){
        fprintf(stderr, "Alokacia pamate zlyhala.\n");
	}

	// nacitanie suboru do retazca
	len=0;
	while((c=fgetc(f)) !=EOF){
        out[len++] = c;
	}
	out[len] = '\0'; // vlozime ukoncovaci znak a tak vytvorime platny C retazec
	fclose(f); // zavrieme subor
	return out;
}

typedef struct vojak

{
    int life;
    int att;
    int def;
    int meno;
    int x;
    int y;
    char c;

}VOJAK;

typedef struct Hrac
{
    VOJAK *armada;
    int pV;
}PLAYER;



void gen(VOJAK* vojak)
{
    vojak->meno = rand()% 1219;

    vojak->life=5;
    vojak->att=0;
    vojak->def=0;
    vojak->x=0;
    vojak->y=0;
    vojak->c='X';

    while(vojak->att+vojak->def!=5)
    {
        vojak->att=rand()%5+1;
        vojak->def=rand()%5+1;
    }

}



void gen_army(VOJAK *armada,int p,int h)
{

    int i;
    for(i=0;i<p;i++)
    {

        gen(&(armada[i]));
    printf("Vojak %d\n Attack: %d\n Defence: %d\n\n",i,armada[i].att,armada[i].def);

    if(h == 1)
    {


        armada[i].y=3;
        armada[i].x=54+(3*i);
    }else{
        armada[i].y=26;
        armada[i].x=54+(3*i);
        }
    }


}



void drawarmy(VOJAK *armada,int p, int ind)
{
    int i;

   for(i=0;i<p;i++)
    {
        if(ind == i)
        {
            init_pair(1, COLOR_WHITE, COLOR_GREEN);
            attron(COLOR_PAIR(1));
            mvprintw(armada[i].y,armada[i].x,"[%c]",armada[i].c);
            attroff(COLOR_PAIR(1));
        }
        else
            mvprintw(armada[i].y,armada[i].x," %c ",armada[i].c);

    }



}



void move_Vojak(char c,VOJAK *v, VOJAK *v2,int *n1, int *n2)
{

    //   mvprintw(16,20,"n1: %d",*n1);
   //    mvprintw(17,20,"n1: %d",*n2);
  //  mvprintw(18,20,"   y: %d",v->y);
  //  mvprintw(20,20,"   life: %d",v->life);

    switch (c)
    {
        case 'w':
            if (v->y > 1)
            v->y--;
            break;
        case 'a':
            if (v->x > 1)
            v->x-=2;
            break;
        case 's':
            if (v->y <= 17)
            v->y++;
            break;
        case 'd':
            if (v->x <= 51)
            v->x+=2;
            break;
        case ' ':
            if(abs(v->x - v2->x) == 1 || abs(v->y - v2->y) == 1)
                {

                    secni(v,v2,1,n1,n2);
                }
            else if(abs(v->x - v2->x) == 2 && abs(v->y - v2->y) == 0 )
                secni(v,v2,1,n1,n2);
                //system("pause");
    }
}

void effect()
{


       WINDOW *b;
    b = newwin(29, 120,0,0);
      init_pair(2, COLOR_WHITE, COLOR_WHITE);

       wattrset(b, 0);
       wbkgd(b,COLOR_PAIR(2));
        wattrset(b, COLOR_PAIR(2));
        box(b, ' ', ' ');

    wrefresh(b);
    usleep(20000);
    clear();



}


void secni(VOJAK *p1,VOJAK *p2,int p,int *m1, int *m2)
{
   mvprintw(16,20,"n1: %d",*m1);



//mvprintw(16,20,"a: %d",a);

effect();



    if (p==1)
    {

        if(p1->att>p2->def)
        {
            p2->life=p2->life-(p1->att-p2->def);
        }
        else if(p1->att<p2->def)
        {



            *m1=*m1-1;
        //    printf("%d",*m1);
      //  system("pause");


             if(*m1<=0) p2->life--;




        }

                  else if(*m1==0)
        {

            p2->life--;
        }

    }
    else if( p==2)
    {

         if(p1->att>p2->def)
        {
            p2->life=p2->life-(p1->att-p2->def);


        }
          else if(p1->att<p2->def)
        {

            *m2=*m2-1;

            if(*m2<=0) p2->life--;
            mvprintw(15,20,"%d",*m2);



        }


                 else if(p1->att==p2->def)
        {
            p2->life--;


        }

    }
    //m1=*m1;
    //m2=b;
   // printf("%d,%d",&m1,&m2);
//system("pause");




}


void move_Vojak2(char c,VOJAK *v, VOJAK *v2,int *n2,int *n1)
{

   //  mvprintw(10,20,"p2 x: %d",v->x);
   // mvprintw(12,20,"   y: %d",v->y);
   // mvprintw(14,20,"   life: %d",v->life);
    switch (c)
    {
      case 'i':
            if (v->y > 1)
            v->y--;
            break;
        case 'j':
            if (v->x > 1)
            v->x-=2;
            break;
        case 'k':
            if (v->y <= 17)
            v->y++;
            break;
        case 'l':
            if (v->x <= 51)
            v->x+=2;
            break;
       case '0':
           if(abs(v->x - v2->x) == 1 || abs(v->y - v2->y) == 1)
           {
                //system("pause");
                secni(v,v2,2,n2,n1);
           }
            else if(abs(v->x - v2->x) == 2 && abs(v->y - v2->y) == 0 )
            {

                 secni(v,v2,2,n2,n1);



            }
    }
}



void drawBox(int h, int w, int x, int y, char * s, char * p1,char * p2, char* p3)
{

    WINDOW *b;
    b = newwin(h, w, x,y);



     init_pair(2, COLOR_WHITE, COLOR_RED);
     //init_pair(3, COLOR_WHITE, COLOR_WHITE);


     wattrset(b, 0);
        wattrset(b, COLOR_PAIR(2));




        box(b, ' ', ' ');
        mvwaddstr(b,1,3,s);
        if (strcmp(p2, "") != 0)
        {
        mvwaddstr(b,3,3,"ATTACK: ");
         mvwaddstr(b,5,3,"DEFENSE: ");
          mvwaddstr(b,7,3,"LIVES: ");

        }
        mvwaddstr(b,3,12,p1);
        mvwaddstr(b,5,12,p2);
        mvwaddstr(b,7,12,p3);
        wrefresh(b);

}


void details(PLAYER hrac,char * mena[200],int h)
{
    if (h == 1)
    {
            mvaddstr(1,3,"PLAYER 1");
    }

}

void drawBattle(int x1, int y1,int x2, int y2,int arenaH,int arenaW)
{
     WINDOW *b;
    b = newwin(arenaH, arenaW, (LINES - arenaH) / 2, (COLS - arenaW) / 2);
      init_pair(6, COLOR_WHITE, COLOR_RED);
      init_pair(4, COLOR_WHITE,COLOR_YELLOW);
        wbkgd(b,COLOR_PAIR(4));
       wattrset(b, 0);
        wattrset(b, COLOR_PAIR(6));
        box(b, ' ', ' ');
        mvwaddstr(b,0,26, "FIGHT");
        mvwaddstr(b,y1,x1, " X ");
        mvwaddstr(b,y2,x2, " X ");


        wrefresh(b);

}

void battle(int vojak1, int vojak2, PLAYER p1, PLAYER p2, int b)
{
    if (b == 1)
    {
    p1.armada[vojak1].x = 27;
    p1.armada[vojak1].y = 2;

    p2.armada[vojak2].x = 27;
    p2.armada[vojak2].y = 17;
    }
     drawBattle( p1.armada[vojak1].x,p1.armada[vojak1].y,p2.armada[vojak2].x,p2.armada[vojak2].y,20,56);
}

char *choices[] = {
			"START GAME",
			"HELP",
			"EXIT"
		  };

int n_choices = sizeof(choices) / sizeof(char *);

void print_menu(WINDOW *menu_win, int highlight)
{
	int x, y, i;


	x = 2;
	y = 2;
	 init_pair(2, COLOR_WHITE, COLOR_RED);

    wattrset(menu_win, COLOR_PAIR(2));
	box(menu_win, ' ', ' ');
	for(i = 0; i < n_choices; ++i)
	{	if(highlight == i + 1) /* High light the present choice */
		{	wattron(menu_win, A_REVERSE);
			mvwprintw(menu_win, y, x, "%s", choices[i]);
			wattroff(menu_win, A_REVERSE);
		}
		else
			mvwprintw(menu_win, y, x, "%s", choices[i]);
		++y;
	}
	wrefresh(menu_win);
}



int mainMenu(){

WINDOW *menu_win;
	int highlight = 1;
	int choice = 0;
	int c;
	int startx = 0;
	int starty = 0;


clear();

cbreak();
startx = (80 - 30) / 2;
starty = (24 - 10) / 2;

menu_win = newwin(10, 37, 12, 44);
keypad(menu_win, TRUE);





refresh();
 print_menu(menu_win, highlight);
	while(1)
	{	 drawBox(4,37,LINES/4,COLS/4+14,"T H E  B A T T L E  O F  F E I ","","","");

	    c = wgetch(menu_win);

		switch(c)
		{	case KEY_UP:
				if(highlight == 1)
					highlight = n_choices;
				else
					--highlight;
				break;
			case KEY_DOWN:
				if(highlight == n_choices)
					highlight = 1;
				else
					++highlight;
				break;
			case 10:
				choice = highlight;
				break;
			default:

				wrefresh(menu_win);
				break;
		}
		print_menu(menu_win, highlight);
		if(choice != 0)
        {
             if (choice == 2)
        {
        clear();
        while(1)
        {


            drawBox(4,37,LINES/4,COLS/4+14,"   I N S T R U C T I O N S ","","","");
            drawBox(6,50,LINES/4+4,COLS/4+7,"   CONTROLS PLAYER 1:  ","MOVE: [W A S D] ATTACK: [SPACEBAR]","","");
            drawBox(6,50,LINES/4+10,COLS/4+7,"   CONTROLS PLAYER 2: ","MOVE: [I J K L] ATTACK: [NUMPAD 0]","","");
            drawBox(6,50,LINES/4+16,COLS/4+7,"   SOLDIER SELECTION: ","NEXT: [SPACEBAR] SELECT: [ENTER]","","");
            mvprintw(29,50,"PRESS ENTER TO RETURN");

             c = wgetch(menu_win);

            if(c == 10)
            {
                refresh();
                clear();
                choice = 0;

              break;
            }
        }
        }
        else if (choice == 3)
            return 1;
        else{break;}

	}
	//mvprintw(29,37,"%s SELECTED, PRESS ENTER TO CONTINUE", choices[choice-1]);

    }

	//clear();



}



int main() {

time_t t = time(NULL);
struct tm tm = *localtime(&t);


int boje = 0;


     FILE * g;
	g = fopen("log.txt", "wt"); // otvarame subor
	if (g == NULL) // kontrolujeme uspesnost otvorenia
		return 0;

	fprintf(g, "START: %d. %d. %d %d:%d:%d\n\n**********************************", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900, tm.tm_hour, tm.tm_min, tm.tm_sec); // zapiseme retazec do suboru
	//




    srand(time(0));
    char* token;
    initscr();
    noecho();
    int i =0;
    int mX,mY;
    char mena[5000][200];
    getmaxyx(stdscr, mX, mY);
    int p;
    char s[8500];
    strcpy(s,read_file("C:\\Users\\Zelko\\Desktop\\Zadania_PROG2\\Zadanie 9\\Zadanie9\\bin\\Debug\\mena2.txt"));
     token = strtok(s, "\n");
    while( token != NULL ) {
            strcpy(mena[i],token);
            token = strtok(NULL, "\n");
            i++;

    }


    int boj = 1;

    i=0;
    p=5;
    char c;
    PLAYER p1,p2;
    int bo = 1;
    VOJAK armada1[p],armada2[p];



    p1.armada = armada1;
    p2.armada = armada2;
    //VOJAK armada[p];

    gen_army(p1.armada,p,1);
    gen_army(p2.armada,p,2);


    //gen_army(p2.armada,p);
    WINDOW *border;
    //gen_army(armada,p);
    int arenaH = 20;
    int arenaW = 56;
    start_color();
    use_default_colors();
    curs_set(0);
    refresh();
    //armada[0].y = 20
    char v1[20],v2[20];
    int j;
    for(j = 0; j < p; j++)
    {
        v1[j] = 'I';
        v2[j] = 'I';
    }


    char sa[30],sd[30],sl[30];
     char sa1[30],sd1[30],sl1[30];

    int k,b=1;
    char v;
    k = 0;
    i = 0;
    if (mainMenu())
        return 0;
    int kv1=0;
    int kv2=0;
     int m1 ;
        int m2 ;
    while(1)
    {



          if(kv1==p || kv2==p)
        {

            clear();
            if(kv2==p)
            {


             drawBox(3,35,15,44,"       ZVITAZIL HRAC 1       ","","","");
                fprintf(g,"       ZVITAZIL HRAC 1       \n");
             fprintf(g, "\n ZOSNULI BOJOVNICI HRACA 1: \n");
            for(int i = 0; i < p; i++)
            {if (p1.armada[i].life==0)
                fprintf(g, "    * %s, %10s= %d ATT= %d DEF= %d\n", mena[p1.armada[i].meno],"LIFE", p1.armada[i].life,p1.armada[i].att,p1.armada[i].def);
            }

             fprintf(g, "\n ZOSNULI BOJOVNICI HRACA 2: \n");
            for(int i = 0; i < p; i++)
            {
                if (p2.armada[i].life==0)
                fprintf(g, "    * %s, %10s= %d ATT= %d DEF= %d\n", mena[p1.armada[i].meno],"LIFE", p2.armada[i].life,p2.armada[i].att,p2.armada[i].def);
            }
                fprintf(g, "POCET SUBOJOV: %d", boje);
                fclose(g);


            }
            else {drawBox(3,35,15,44,"       ZVITAZIL HRAC 2       ","","","");
                fprintf(g,"       ZVITAZIL HRAC 2       ");

             fprintf(g, "\n ZOSNULI BOJOVNICI HRACA 1: \n");
            for(int i = 0; i < p; i++)
            {if (p1.armada[i].life==0)
                fprintf(g, "    * %s, %10s= %d ATT= %d DEF= %d\n", mena[p1.armada[i].meno],"LIFE", p1.armada[i].life,p1.armada[i].att,p1.armada[i].def);
            }

             fprintf(g, "\n ZOSNULI BOJOVNICI HRACA 2: \n");
            for(int i = 0; i < p; i++)
            {
                if (p2.armada[i].life==0)
                fprintf(g, "    * %s\n", mena[p1.armada[i].meno]);
            }

             fprintf(g, "\nPOCET SUBOJOV: %d",boje);
                fclose(g);

            }
            return 0;



        }
        //getchar();



        halfdelay(1);
        while(b < 3){


        if(b == 1)
        {

            mvprintw(0,0,"%d",i);
            v = getch();
        drawBox(3,35,0,44,"PLAYER 1 TURN, CHOOSE SOLDIER","","","");


        if (v == ' ' && i <= p-1 && v != '\n')
                i++;
        else if (i>=p) i=0;




        }

         if(b == 2)
        {
         drawBox(3,35,0,44,"PLAYER 2 TURN, CHOOSE SOLDIER","","","");
         v = getch();
         if (v == ' ' && k <= p-1  && v !='\n')
                k++;
        else if (k>=p) k=0;

            m1=abs(p1.armada[i].att-p2.armada[k].def);
            m2=abs(p2.armada[k].att-p1.armada[i].def);


        }






        drawBox(6,25,0,0,"PLAYER 1",v1,"","");
        drawBox(15,20,5,0,mena[p1.armada[i].meno],itoa(p1.armada[i].att,sa,10),itoa(p1.armada[i].def,sd,10),itoa(p1.armada[i].life,sl,10));
        strcpy(sa,"");
        strcpy(sd,"");
        strcpy(sl,"");

        drawBox(6,25,0,mY-25,"PLAYER 2",v2,"","");
        drawBox(15,20,5,mY-20,mena[p2.armada[k].meno],itoa(p2.armada[k].att,sa1,10),itoa(p2.armada[k].def,sd1,10),itoa(p2.armada[k].life,sl1,10));

        drawBox(arenaH,arenaW, (LINES - arenaH) / 2, (COLS - arenaW) / 2,"","","","");

        //mvaddstr(7, 3,"AFAF");
        //details(p1,mena,1);
        drawarmy(p1.armada,p,i);
        drawarmy(p2.armada,p,k);






                    //system("pause");







        //move_Vojak(getch(),&(p1.armada[0]));

        //move_Vojak2(getch(),&(armada[1]));
        refresh();
        //clear();

        if (v == '\n')
        {
            if(p1.armada[i].c == 'O' && b==1)
                drawBox(5,30,23,90,mena[p1.armada[i].meno],"ZAHYNUL V BOJI","","");

            else if(p2.armada[k].c == 'O' && b==2)
                drawBox(5,30,23,90,mena[p2.armada[k].meno],"ZAHYNUL V BOJI","","");
            else{
                    if(b==1)
                    {
                        drawBox(5,30,23,90,mena[p1.armada[i].meno],"SELECTED","","");

                    }
                    else{
                        drawBox(5,30,23,90,mena[p2.armada[k].meno],"SELECTED","","");

                        }
                b++;

            }


        }


        }

        if (boj){
                    boje++;
                    boj=0;
             fprintf(g, "\n ARMADA HRACA 1: \n");
            for(int i = 0; i < p; i++)
            {if (p1.armada[i].life!=0)
                fprintf(g, "    * %s, %10s= %d ATT= %d DEF= %d\n", mena[p1.armada[i].meno],"LIFE", p1.armada[i].life,p1.armada[i].att,p1.armada[i].def);
            }

             fprintf(g, "\n ARMADA HRACA 2: \n");
            for(int i = 0; i < p; i++)
            {
                if (p2.armada[i].life!=0)
                fprintf(g, "    * %s, %10s= %d ATT= %d DEF= %d\n", mena[p2.armada[i].meno],"LIFE", p2.armada[i].life,p2.armada[i].att,p2.armada[i].def);
            }


            fprintf(g, "SUBOJ (%d KOLO):\n", boje);
            fprintf(g, "    PLAYER 1 ZVOLIL BOJOVNIKA %s(LIFE = %d ATT = %d DEF = %d)\n", mena[p1.armada[i].meno], p1.armada[i].life, p1.armada[i].att, p1.armada[i].def);
            fprintf(g, "    PLAYER 2 ZVOLIL BOJOVNIKA %s(LIFE = %d ATT = %d DEF = %d)\n", mena[p2.armada[k].meno], p2.armada[k].life, p2.armada[k].att, p2.armada[k].def);


        }


        if (b== 3)
        {





            //sleep(1);
            drawBox(5,30,23,90,"","","","");
            drawBox(3,35,0,44,"        BATTLE PHASE         ","","","");
        //clear();





        battle(i,k,p1,p2,bo);
        bo = 0;
        //halfdelay(1);
        if (m1 <= 0)
                    m1=abs(p1.armada[i].att-p2.armada[k].def);
        if (m2 <= 0)
                    m2=abs(p2.armada[k].att-p1.armada[i].def);

        move_Vojak(getch(),&p1.armada[i], &p2.armada[k], &m1,&m2);
        move_Vojak2(getch(), &p2.armada[k], &p1.armada[i], &m2,&m1);


          drawBox(6,25,0,0,"PLAYER 1",v1,"","");
        drawBox(15,20,5,0,mena[p1.armada[i].meno],itoa(p1.armada[i].att,sa,10),itoa(p1.armada[i].def,sd,10),itoa(p1.armada[i].life,sl,10));

         drawBox(6,25,0,mY-25,"PLAYER 2",v2,"","");
        drawBox(15,20,5,mY-20,mena[p2.armada[k].meno],itoa(p2.armada[k].att,sa1,10),itoa(p2.armada[k].def,sd1,10),itoa(p2.armada[k].life,sl1,10));




          if (p1.armada[i].life<=0)
            {
                kv1++;
                p1.armada[i].life=0;

                b=1;
                v1[i] = 'X';
                p1.armada[i].c='O';
                drawBox(5,30,23,90,"PLAYER 2 WINS BATTLE","","","");
                fprintf(g, "ZVITAZIL BOJOVNIK %s HRACA 1 \n\n" ,mena[p1.armada[i].meno]);
                 fprintf(g, "**********************************\n");
                p1.armada[i].y=3;
                p1.armada[i].x=54+(3*i);

                p2.armada[k].y=26;
                p2.armada[k].x=54+(3*i);
                bo = 1;
                boj = 1;


            }

             if (p2.armada[k].life<=0)
            {
                kv2++;
                p2.armada[k].life=0;

                b=1;
                 v2[k] = 'X';
                p2.armada[k].c='O';

                drawBox(5,30,23,90,"PLAYER 1 WINS BATTLE","","","");
                fprintf(g, "ZVITAZIL BOJOVNIK %s HRACA 2 \n\n",mena[p2.armada[k].meno]);
                 fprintf(g, "**********************************\n");
                p2.armada[k].y=26;
                p2.armada[k].x=54+(3*k);
                p1.armada[i].y=3;
                p1.armada[i].x=54+(3*i);
                bo = 1;
                boj = 1;

            }

                  drawBox(15,20,5,0,mena[p1.armada[i].meno],itoa(p1.armada[i].att,sa,10),itoa(p1.armada[i].def,sd,10),itoa(p1.armada[i].life,sl,10));
          drawBox(15,20,5,mY-20,mena[p2.armada[k].meno],itoa(p2.armada[k].att,sa1,10),itoa(p2.armada[k].def,sd1,10),itoa(p2.armada[k].life,sl1,10));

        refresh();


        }

    }



}
