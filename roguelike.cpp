#include <Windows.h>
#include <cstring>
#include <iostream>
#include <string>
#include <curses.h>
#include <roguelib.h>

using namespace std;

int main()
{
	// clés du clavier et sa représentation interne
	const char KEY_quit		= 'q';	// touche pour quitter en minuscule
	const char KEY_QUIT		= 'Q';	// touche pour quitter en majuscule maudit caps lock
	const char KEY_SPACE	= ' ';	// touche pour éteindre le feu
	const char WALL			= 'X';
	const char PLAYER		= 'O';
	const char FIRE			= '^';
	const char WATER		= 'W';
	const char WATER_PICKUP = 'w';
	const char ELECTRICITY	= 'Z';
	const char TUNNEL	 	= 'T';  //tunnel #1 - quickly jump to end of the tunnel
	const char SWITCH		= 'S';  //swich - interrupteur - action toggle sur un objet
	const char KEY			= 'K';  //clé pour éteindre le computer qui est l'objectif final

										// vous pouvez utiliser les valeurs déjà connues:
	// COLOR_BLACK   0,   COLOR_RED     1,   COLOR_GREEN   2,    COLOR_YELLOW  3,   COLOR_BLUE    4
	// COLOR_MAGENTA 5,   COLOR_CYAN    6,   COLOR_WHITE   7
 	init_color(/* numéro de la nouvelle couleur */ 8, /* rouge */ 1000, /* vert */ 250, /* bleu */ 250);  	// couleur rouge vif 

	init_pair('R', 8,			   COLOR_BLACK);   // 8 = rouge vif
	init_pair('G', COLOR_GREEN,    COLOR_BLACK);
	init_pair('W', COLOR_BLUE,     COLOR_BLACK);
	init_pair('g', COLOR_BLACK,    COLOR_GREEN);  	// assigne la couleur noir sur vert à la valeur 'g'
	init_pair('Z', COLOR_YELLOW,   COLOR_BLACK);
	init_pair('T', COLOR_CYAN,	   COLOR_BLACK);
	init_pair('M', COLOR_MAGENTA,  COLOR_BLACK);



	// dimensions de la grille
	const int COLS = 160;
	const int ROWS = 51;

	// level design
	char grid[ROWS][COLS + 1] =
	{
	//             1         2         3         4         5         6         7         8         9         10        11        12        13        14        15 
	//   01234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890
		"XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",  //0
	    "XXX                                    XXX                                                                                                                     X",  //1
	    "XXX                                    XXX             XXXXXXXXXXXXX               XXXSXXXXXXXXX                 XXXXXXXXXXXXXX               XXXXXXXXXXXX     X",  //2
		"XXX                                    XXX             XXXXXXXXXXXXX               XXXXXXXXXXXXX                 XXXXXXXXXXXXXX               XXXXXXXXXXXX     X",  //3
	    "XXX                                    XXX             X          SXXXXXX     XXXXXX           X                 X            XXXXXX     XXXXXX          X     X",  //4
		"XXX                                    XXX             X  XXXXXXXXXXXXXXXXXXXXXXXXXX           X                 X            XXXXXX     XXXXXX          X     X",  //5
		"XXX                                    XXX             X  X       Z  Z          Z  Z           X                 X                                       X     X",  //6
		"XXX                                    XXX             X  ZZZ      Z  Z          Z  Z          X                 X                   ***                 X     X",  //7
		"XXX                                    XXX             X  ZZZ     Z  Z          Z  Z           X                 X                                       X     X",  //8
		"XXXXXXXXXXXXXXXX        XXXSXXXXXXXXXXXXXX             X  ZZZ      Z  Z          Z  Z          X                 X               XXX     XXX             X     X",  //9
		"XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX             X  ZZZ     Z  Z  Z       Z  Z  Z        X                 X               XwwwwwwwwwX             X     X",  //10
		"XXXXXXXXXXXXXXXX        XXXXXXXXXXXXXXXXXX             X  ^^^                                  X                 X               XXXXXXXXXXX             X     X",  //11
		"XXX                                    XXX             XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX                 XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX     X",  //12
		"XXX                                    SXX                                                                                                                     X",  //13
		"XXX                                    XXX                                                                                                                     X",  //14
		"XXX                                    XXX                                                                                                                     X",  //15
		"XXX                                    XXX             XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX                 XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX     X",  //16
		"XXX                                     X              X                                       X                 X                       ^^^^^           X     X",  //17
		"XXX                                     X              X                Z     Z                X                 X    XXXXXXXXXXXXXXXXXXXXXXXX           X     X",  //18
		"XXX                                     X              X                Z     Z                X                 X    z    z   z   z  z  z   XXXXXXXXXXXXX     X",  //19
		"XXX                                     X              X                Z     Z                X                 X       z   z   z  z   z    X           X     X",  //20
		"XXX                                     X              X                Z     Z                X                 X    z   z    z   z  z  z   X           X     X",  //21
		"XXX                                    XXX             X                Z^^^^^Z                X                 X      z               z    XXXXXXXXXXXXX     X",  //22
		"XXX                                    XXX             X           XXXXXXXXXXXXXXXXXZZZZZZZZ^^^X                 X    z   z  XXXXXXXXXXXXXXXXX           X     X",  //23
		"XXX                                    XXX             X           XXXXXX     XXXXXXS          X                 X           XXXXXX     XXXXXX     K     X     X",  //24
		"XXX                                    XXX             XXXXXXXXXXXXX               XXXXXXXXXXXXX                 XXXXXXXXXXXXX               XXXXXXXXXXXXX     X",  //25
		"XXX                                    XXX             XXXXXXXXXSXXX               XXXXXXXXXXXXX                 XXXXXXXXXSXXX               XXXXXXXXXXXXX     X",  //26
		"XXX                                    XXX                                                                                                                     X",  //27
		"XXX                                    XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",  //28
		"XXX                                    XXX                                                                                                                     X",  //29
		"XXX                                    XXX                                                                                                                     X",  //30
		"XXX                                    XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",  //31
		"XXX                                    XXX                 ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ         ZZZZZZZZZZZ      ^^^^^                      X",  //32
		"XXX                                    XXX             ZZZ ZZZZZZZZZZZZZZZZZZZZZZZZZ     ZZZZZZZZ      ZZZ ZZZZZZZ ZZZZZZ           ^^^^^   XXXXXXXXXXXXXX     X",  //33
		"XXX                                    XXX             ZZZ ZZZZ             ZZZZZZZZ ZZZ ZZZZZZZZ ZZZZ ZZZ ZZZZZZZ ZZZZZZ ZZZZ      ^^^^^   X            X     X",  //34
		"XXX                                    XXX             ZZZ ZZZZ ZZZZZZZZZZZ ZZZZZZZZ ZZZ ZZZZZZZZ ZZZZ ZZZ   ZZZZZ ZZZZZZ ZZZZ      ^^^^^   X  COMPUTER  X     X",  //35
		"XXX                                    XXX             ZZZ ZZZZ     ZZZZZZZ ZZZZZZZZ ZZZ ZZZZZZZZ ZZZZ ZZZZZ ZZZZZ ZZZZZZ ZZZZ      ^^^^^   XXXXXXXXXXXXXX     X",  //36
		"XXX                                    SXX             ZZZ ZZZZZZZZ ZZZZZZZ ZZZZZZZZ ZZZ ZZZZZZZZ ZZZZ    ZZ ZZZZZ ZZZZZZ ZZZZ      ^^^^^        X  X          X",  //37
		"XXX                                    XXX             ZZZ    ZZZZZ ZZZZZZZ   ZZZZZZ ZZZ ZZZZZZZZ ZZZZZZZ ZZ ZZZZZ        ZZZZ      ^^^^^    XXXXXXXXXXXX      X",  //38
		"XXX                                    XXX             ZZZZZZ ZZZZZ ZZZZZZZZZ ZZZZZZ ZZZ ZZZZZZZZ ZZZZZZZ ZZ ZZZZZZZZZZZZZZZZZ      ^^^^^    XxxxxxxxxxxXXXX   X",  //39
		"XXX                                    XXXXXXXXX       ZZZZZZ ZZZZZ ZZZZZZZZZ ZZZZZZ ZZZ ZZZZZZZZ ZZZZZZZ ZZ    ZZZZZZZZZZZZZZ      ^^^^^    XXXXXXXXXXXX  X   X",  //40
		"XXX                                    XTXXX  T        ZZZ    ZZZZZ ZZZZZZZZZ ZZZZZZ ZZZ ZZZZZZZZ ZZZZZZZ ZZZZZ ZZZZZZZZZZZZZZ      ^^^^^                  X   X",  //41
		"XXX                                    XXXXXXXXX       ZZZ ZZZZZZZZ ZZZZ      ZZZZZZ ZZZ ZZZZZZZZ ZZZZZZZ    ZZ ZZZZZZZZZZZZZZ      ^^^^^                  X   X",  //42
		"XXX                                    XXX             ZZZ          ZZZZ ZZZZZZZZZZZ ZZZ          ZZZZZZZZZZ ZZ ZZZZZZZZZZZZZZ      ^^^^^                  S   X",  //43
		"XXX                                    XXX             ZZZZZZZZZZZZZZZZZ             ZZZZZZZZZZZZZZZZZZZZZZZ    ZZZZZZZZZZZZZZ      ^^^^^                      X",  //44
		"XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",  //45
		"X                X                     X                                                                                                           X           X",  //46
		"X Vie 0123456789 X                     X     Frgu doit eteindre l'ordi de David Lavoie!        Utilisez les fleches et Space pour y arriver.       X           X",  //47
		"X                X                     X                                                                                                           X           X",  //48
		"XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",  //49
		"                                                                                                                                                                ",  //50
	};

	// tableau de couleurs
	char colors[ROWS][COLS + 1] =
	{
	//             1         2         3         4         5         6         7         8         9         10        11        12        13        14        15 
	//   01234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890
		"gggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggg",  //0
		"ggg                                    ggg                                                                                                                     g",  //1
		"ggg                                    ggg             ggggggggggggg               ggggggggggggg                 gggggggggggggg               gggggggggggg     g",  //2
		"ggg                                    ggg             ggggggggggggg               ggggggggggggg                 gggggggggggggg               gggggggggggg     g",  //3
		"ggg                                    ggg             g          ggggggg     gggggg           g                 g            gggggg     gggggg          g     g",  //4
		"ggg                                    ggg             g  gggggggggggggggggggggggggg           g                 g            gggggg     gggggg          g     g",  //5
		"ggg                                    ggg             g  g       Z  Z          Z  Z           g                 g                                       g     g",  //6
		"ggg                                    ggg             g  ZZZ      Z  Z          Z  Z          g                 g                                       g     g",  //7
		"ggg                                    ggg             g  ZZZ     Z  Z          Z  Z           g                 g                                       g     g",  //8
		"gggggggggggggggg        gggggggggggggggggg             g  ZZZ      Z  Z          Z  Z          g                 g               ggg     ggg             g     g",  //9
		"gggggggggggggggggggggggggggggggggggggggggg             g  ZZZ     Z  Z  Z       Z  Z  Z        g                 g               gWWWWWWWWWg             g     g",  //10
		"gggggggggggggggg        gggggggggggggggggg             g  RRR                                  g                 g               gWWWWWWWWWg             g     g",  //11
		"ggg                                    ggg             ggggggggggggggggggggggggggggggggggggggggg                 ggggggggggggggggggggggggggggggggggggggggg     g",  //12
		"ggg                                    ggg                                                                                                                     g",  //13
		"ggg                                    ggg                                                                                                                     g",  //14
		"ggg                                    ggg                                                                                                                     g",  //15
		"ggg                                    ggg             ggggggggggggggggggggggggggggggggggggggggg                 ggggggggggggggggggggggggggggggggggggggggg     g",  //16
		"ggg                                     g              g                                       g                 g                       RRRRR           g     g",  //17
		"ggg                                     g              g                Z     Z                g                 g    gggggggggggggggggggggggg           g     g",  //18
		"ggg                                     g              g                Z     Z                g                 g    Z    Z   Z   Z  Z  Z   ggggggggggggg     g",  //19
		"ggg                                     g              g                Z     Z                g                 g       Z   Z   Z  Z   Z    g           g     g",  //20
		"ggg                                     g              g                Z     Z                g                 g    Z   Z    Z   Z  Z  Z   g           g     g",  //21
		"ggg                                    ggg             g                ZRRRRRZ                g                 g      Z               Z    ggggggggggggg     g",  //22
		"ggg                                    ggg             g           gggggggggggggggggZZZZZZZZRRRg                 g    Z   Z  ggggggggggggggggg           g     g",  //23
		"ggg                                    ggg             g           gggggg     ggggggg          g                 g           gggggg     gggggg           g     g",  //24
		"ggg                                    ggg             ggggggggggggg               ggggggggggggg                 ggggggggggggg               ggggggggggggg     g",  //25
		"ggg                                    ggg             ggggggggggggg               ggggggggggggg                 ggggggggggggg               ggggggggggggg     g",  //26
		"ggg                                    ggg                                                                                                                     g",  //27
		"ggg                                    ggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggg",  //28
		"ggg                                    ggg                                                                                                                     g",  //29
		"ggg                                    ggg                                                                                                                     g",  //30
		"ggg                                    ggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggg",  //31
		"ggg                                    ggg             ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ      RRRRR                      g",  //32
		"ggg                                    ggg             ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ      RRRRR   XXXXXXXXXXXXXX     g",  //33
		"ggg                                    ggg             ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ      RRRRR   XZZZZZZZZZZZZX     g",  //34
		"ggg                                    ggg             ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ      RRRRR   XZZZZZZZZZZZZX     g",  //35
		"ggg                                    ggg             ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ      RRRRR   XXXXXXXXXXXXXX     g",  //36
		"ggg                                    ggg             ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ      RRRRR        Z  Z          g",  //37
		"ggg                                    ggg             ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ      RRRRR    XXXXXXXXXXXX      g",  //38
		"ggg                                    ggg             ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ      RRRRR    XxxxxxxxxxxXggg   g",  //39
		"ggg                                    ggggggggg       ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ      RRRRR    XXXXXXXXXXXX  g   g",  //40
		"ggg                                    gTggg  T        ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ      RRRRR                  g   g",  //41
		"ggg                                    ggggggggg       ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ      RRRRR                  g   g",  //42
		"ggg                                    ggg             ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ      RRRRR                  S   g",  //43
		"ggg                                    ggg             ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ      RRRRR                      g",  //44
		"gggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggg",  //45
		"g                g GGGGGGGGGGGGGGGGGGGGgGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGg",  //46
		"g Vie 0123456789 g GGGGGGGGGGGGGGGGGGGGgGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGg",  //47
		"g                g GGGGGGGGGGGGGGGGGGGGgGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGg",  //48
		"gggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggg",  //49
		"                                                                                                                                                                ",  //50
	};



	//  Ouverture d'un écran positionné dans le haut à gauche de l'écran, prêt à être redimensionné par le jeu
	HWND console = GetConsoleWindow();
	RECT r;
	GetWindowRect(console, &r);								//stores the console's current dimensions
	MoveWindow(console, 1, 1, 800, 600, TRUE);        		//MoveWindow(window_handle, x, y, width, height, redraw_window);
	SetConsoleTitle(TEXT("Le monde merveilleux de Charles"));
	resize_term( 53, 160);									//Agrandissement de l'écran pour tout voir, lines, cols

	

	// Paramètres du jeu
	int y = 2;						// Position de départ: ligne
	int x = 5;		    		    // Position de départ: colonne
	int life = 10;					// nombre de vie
	bool victory = false;			// variable si joueur a gagné, ne peut qu'être mit à true si le joueur atteint l'objectif final, d'éteindre le computer
	
	// Variables d'états de la situation durant le jeu
	bool Debug_Mode = true;			// toggle qui affiche ou non les coordonnées du joueur
	int i = 0;						// sert à boucle for/next
	bool On_Fire = false;			// joueur sur position en feu
	int Conteneur_eau = 0;			// Accumulateur d'eau
	int Conteneur_eau_max = 5;		// Maximum d'eau qu'un conteneur d'eau peut contenir

	bool Salle_1_Porte1 = true;		// Salle 1 Porte 1 pour aller à la Salle 1 plus loin
	bool Salle_1_Porte2 = true;		// Salle 1 Porte 2 pour aller à la Salle 2
	bool Salle_1_Porte3 = true;		// Salle 1 Porte 3 pour aller à la Salle 3 et atteindre l'objectif final
	bool Salle_2_Porte1 = true;		// Salle 2 Porte 1 pour aller dans première salle pour flipper premier mur du stockroom
	bool Salle_2_Porte2 = true;		// Salle 2 Porte 2 pour aller dans deuxième salle pour flipper deuxième mur du stockroom
	bool Salle_2_Porte3 = true;		// Salle 2 Porte 3 pour aller dans stockroom
	bool Salle_2_Stockroom_Porte1 = true;  // swtich doit avoir été flippé à false pour obtenir clé
	bool Salle_2_Stockroom_Porte2 = true;  // swtich doit avoir été flippé à false pour obtenir clé
	
	bool Salle_2_Stockroom_Key	  = false;  // Salle 2 clé afin de permettre d'atteindre l'objectif final
	
	string message_info    = " ";	// Variable servant à envoyer des messages au joueur
	string message_action1 = " ";	// Variable servant à aviser le joueur des différentes actions qu'il peut prendre
	string message_action2 = " ";	// Variable servant à aviser le joueur des différentes actions qu'il peut prendre


	bool quit = false;
	do
	{
		// fait UNE COPIE SEULEMENT du résultat du level + position du joueur
		char render[ROWS][COLS + 1];
		memcpy(render, grid, sizeof(grid));    // destination, source, size
		render[y][x] = PLAYER;
		// fin de la copie de sauvegarde 

				
		message_action1 = " O = Frgu | ^ = Feu | W = eau | Z = Électricité | X = Mur | S = Switch pour ouvrir la porte | T = Tunnel | K = Clé                                              ";    // Variable servant à envoyer des messages au joueur
		if (grid[y][x] == FIRE)
		{	
			message_info = message_info + "Appuyez sur Space pour eteindre le feu, faites vite!!!  J'espère que vous avez de l'eau avec vous!";					  
			On_Fire = true;
		}
		if (grid[y][x] == ELECTRICITY)
		{
			flash();
			message_info = message_info + "                   Aucune defense possible, sauvez-vous rapidement !!!    ";
			--life;		// Perte une vie instantanée
		}
		if (grid[y][x] == TUNNEL)
		{
			message_info = message_info + "                   Weeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee! Et hop!!!   ";
			if (y == 41 && (x == 40 || x == 46))
				if ( x == 40) { x = 46; } else { x = 40; };

		}
		if (grid[y][x] == WATER_PICKUP)
		{
			message_info = message_info + "Ramasser une petite quantite avec Space peut eteindre ses soucis dans le feu de l'action";
		}


		// contruction du message_action que peut prendre le joueur
		message_action1 = "Eau = " + to_string(Conteneur_eau) + "  " + "Cle  = " + to_string(Salle_2_Stockroom_Key);
		message_action2 = "                                                                                                           ";
		// affichage du message_action que peut prendre le joueur  
		for (i = 0; i < message_action1.length(); i++) 
			{
				grid[47][20 + i] = message_action1[i];
			}
		for (i = 0; i < message_action2.length(); i++)
		{
			grid[47][40 + i] = message_action2[i];
		}


		//
		//  AFFICHE LE SNAPSHOT DE LA GRID 
		//
		// *** Dessine le level DU TOP LEFT AU BOTTOM RIGHT et le message sur la dernière ligne seulement
		roguelib::Draw(render, colors, COLS, ROWS, message_info);
		roguelib::DrawHorizontalBar(6, 47, life, '+', life > 2 ? 'G' : 'R');
		/

		if (victory) { Sleep(3000);  break; }   // lorsque but ultime atteint, je laisse système repeindre la matrice et je sors

		// lit les touches de clavier
		switch (roguelib::ReadKey())
		{
			case KEY_QUIT:  quit = true; break;
			case KEY_quit:	quit = true; break;
			case KEY_UP:    if (y > 0 && grid[y - 1][x] != WALL) --y;			if (On_Fire) { flash(); --life; On_Fire = false; } break;
			case KEY_DOWN:  if (y < ROWS - 1 && grid[y + 1][x] != WALL) ++y;	if (On_Fire) { flash(); --life; On_Fire = false; } break;
			case KEY_LEFT:  if (x > 0 && grid[y][x - 1] != WALL) --x;			if (On_Fire) { flash(); --life; On_Fire = false; } break;
			case KEY_RIGHT: if (x < COLS - 1 && grid[y][x + 1] != WALL) ++x;	if (On_Fire) { flash(); --life; On_Fire = false; } break;
			case KEY_SPACE: 
					if (grid[y][x] == KEY)
						{
							if (!Salle_2_Stockroom_Key) 
							{ 
								Salle_2_Stockroom_Key = true;
								colors[y][x] = ' ';
								grid[y][x] = ' ';
							} else 
							{	
								Salle_2_Stockroom_Key = false;
								colors[y][x] = 'R';
								grid[y][x] = 'K';
							}
						}
					if (grid[y][x] == FIRE)														// Au feu
					{ 
						if (Conteneur_eau > 0)
						{
							colors[y][x]	= ' ';
							grid[y][x]		= ' ';
							--Conteneur_eau;
							On_Fire = false;
						}
						else
						{
							flash();
							--life;
						}
						break;
					} 
					if (grid[y][x] == WATER_PICKUP)												// Ramasser de l'eau jusqu'au maximum du contenant
					{
						if (Conteneur_eau == 0)
						{
							// contruction du message_action que peut prendre le joueur
							message_action1 = "Eau = " + to_string(Conteneur_eau) + "  " + "Cle  = " + to_string(Salle_2_Stockroom_Key);
							message_action2   = "  Vous venez de prendre une chaudiere, maintenant continuez en appuyant sur Space pour ramasser de l'eau   ";
							// affichage du message_action que peut prendre le joueur  
							for (i = 0; i < message_action1.length(); i++)
							{
								grid[47][20 + i] = message_action1[i];
							}
							for (i = 0; i < message_action2.length(); i++)
							{
								grid[47][40 + i] = message_action2[i];
							}

						}
						if (Conteneur_eau < Conteneur_eau_max) {Conteneur_eau = Conteneur_eau + 1;}
					}	
					if (grid[y][x] == SWITCH)													// Gestion des portes
					{
						if (y == 9 && x == 27)  // Salle_1_Switch1  (horizontale)
						{ 
							if (!Salle_1_Porte1)
								{
								colors[10][16] = 'g'; grid[10][16] = 'X';
								colors[10][17] = 'g'; grid[10][17] = 'X';
								colors[10][18] = 'g'; grid[10][18] = 'X';
								colors[10][19] = 'g'; grid[10][19] = 'X';
								colors[10][20] = 'g'; grid[10][20] = 'X';
								colors[10][21] = 'g'; grid[10][21] = 'X';
								colors[10][22] = 'g'; grid[10][22] = 'X';
								colors[10][23] = 'g'; grid[10][23] = 'X';
								Salle_1_Porte1 = true;    // porte affichée-fermé pour le passage
							}
							else
							{
								colors[10][16] = ' '; grid[10][16] = ' ';
								colors[10][17] = ' '; grid[10][17] = ' ';
								colors[10][18] = ' '; grid[10][18] = ' ';
								colors[10][19] = ' '; grid[10][19] = ' ';
								colors[10][20] = ' '; grid[10][20] = ' ';
								colors[10][21] = ' '; grid[10][21] = ' ';
								colors[10][22] = ' '; grid[10][22] = ' ';
								colors[10][23] = ' '; grid[10][23] = ' ';
								Salle_1_Porte1 = false;   // porte supprimée-ouvert pour le passage
							}
						}
						if (y == 13 && x == 39)  // Salle_1_Switch2  (verticale)
						{
							if (!Salle_1_Porte2)
							{
								colors[17][40] = 'g'; grid[17][40] = 'X';
								colors[18][40] = 'g'; grid[18][40] = 'X';
								colors[19][40] = 'g'; grid[19][40] = 'X';
								colors[20][40] = 'g'; grid[20][40] = 'X';
								colors[21][40] = 'g'; grid[21][40] = 'X';
								Salle_1_Porte2 = true;    // porte affichée-fermé pour le passage
							}
							else
							{
								colors[17][40] = ' '; grid[17][40] = ' ';
								colors[18][40] = ' '; grid[18][40] = ' ';
								colors[19][40] = ' '; grid[19][40] = ' ';
								colors[20][40] = ' '; grid[20][40] = ' ';
								colors[21][40] = ' '; grid[21][40] = ' ';
								Salle_1_Porte2 = false;   // porte supprimée-ouvert pour le passage
							}
						}
						if (y == 2 && x == 86)  // Salle_2_Switch1  (horizontale)
						{
							if (!Salle_2_Porte1)
							{
								colors[5][73] = 'g'; grid[5][73] = 'X';
								colors[5][74] = 'g'; grid[5][74] = 'X';
								colors[5][75] = 'g'; grid[5][75] = 'X';
								colors[5][76] = 'g'; grid[5][76] = 'X';
								colors[5][77] = 'g'; grid[5][77] = 'X';
								Salle_2_Porte1 = true;    // porte affichée-fermé pour le passage
							}
							else
							{
								colors[5][73] = ' '; grid[5][73] = ' ';
								colors[5][74] = ' '; grid[5][74] = ' ';
								colors[5][75] = ' '; grid[5][75] = ' ';
								colors[5][76] = ' '; grid[5][76] = ' ';
								colors[5][77] = ' '; grid[5][77] = ' ';
								Salle_2_Porte1 = false;   // porte supprimée-ouvert pour le passage
							}
						}
						if (y == 4 && x == 66)  // Salle_2_Stockroom_Switch1  (horizontale) 1ère des 2 portes dans stockroom
						{
							if (!Salle_2_Stockroom_Porte1)
							{
								colors[19][142] = 'g'; grid[19][142] = 'X';
								colors[19][143] = 'g'; grid[19][143] = 'X';
								colors[19][144] = 'g'; grid[19][144] = 'X';
								colors[19][145] = 'g'; grid[19][145] = 'X';
								colors[19][146] = 'g'; grid[19][146] = 'X';
								colors[19][147] = 'g'; grid[19][147] = 'X';
								colors[19][148] = 'g'; grid[19][148] = 'X';
								colors[19][149] = 'g'; grid[19][149] = 'X';
								colors[19][150] = 'g'; grid[19][150] = 'X';
								colors[19][151] = 'g'; grid[19][151] = 'X';
								colors[19][152] = 'g'; grid[19][152] = 'X';
								Salle_2_Stockroom_Porte1 = true;    // porte affichée-fermé pour le passage
							}
							else
							{
								colors[19][142] = ' '; grid[19][142] = ' ';
								colors[19][143] = ' '; grid[19][143] = ' ';
								colors[19][144] = ' '; grid[19][144] = ' ';
								colors[19][145] = ' '; grid[19][145] = ' ';
								colors[19][146] = ' '; grid[19][146] = ' ';
								colors[19][147] = ' '; grid[19][147] = ' ';
								colors[19][148] = ' '; grid[19][148] = ' ';
								colors[19][149] = ' '; grid[19][149] = ' ';
								colors[19][150] = ' '; grid[19][150] = ' ';
								colors[19][151] = ' '; grid[19][151] = ' ';
								colors[19][152] = ' '; grid[19][152] = ' ';
								Salle_2_Stockroom_Porte1 = false;   // porte supprimée-ouvert pour le passage
							}
						}
						if (y == 24 && x == 84)  // Salle_2_Stockroom_Switch2  (horizontale) 2ème des 2 portes dans stockroom
						{
							if (!Salle_2_Stockroom_Porte2)
							{
								colors[22][142] = 'g'; grid[22][142] = 'X';
								colors[22][143] = 'g'; grid[22][143] = 'X';
								colors[22][144] = 'g'; grid[22][144] = 'X';
								colors[22][145] = 'g'; grid[22][145] = 'X';
								colors[22][146] = 'g'; grid[22][146] = 'X';
								colors[22][147] = 'g'; grid[22][147] = 'X';
								colors[22][148] = 'g'; grid[22][148] = 'X';
								colors[22][149] = 'g'; grid[22][149] = 'X';
								colors[22][150] = 'g'; grid[22][150] = 'X';
								colors[22][151] = 'g'; grid[22][151] = 'X';
								colors[22][152] = 'g'; grid[22][152] = 'X';
								Salle_2_Stockroom_Porte2 = true;    // porte affichée-fermé pour le passage
							}
							else
							{
								colors[22][142] = ' '; grid[22][142] = ' ';
								colors[22][143] = ' '; grid[22][143] = ' ';
								colors[22][144] = ' '; grid[22][144] = ' ';
								colors[22][145] = ' '; grid[22][145] = ' ';
								colors[22][146] = ' '; grid[22][146] = ' ';
								colors[22][147] = ' '; grid[22][147] = ' ';
								colors[22][148] = ' '; grid[22][148] = ' ';
								colors[22][149] = ' '; grid[22][149] = ' ';
								colors[22][150] = ' '; grid[22][150] = ' ';
								colors[22][151] = ' '; grid[22][151] = ' ';
								colors[22][152] = ' '; grid[22][152] = ' ';
								Salle_2_Stockroom_Porte2 = false;   // porte supprimée-ouvert pour le passage
							}
						}
						if (y == 26 && x == 64)  // Salle_2_Switch2  (horizontale)
						{
							if (!Salle_2_Porte2)
							{
								colors[23][73] = 'g'; grid[23][73] = 'X';
								colors[23][74] = 'g'; grid[23][74] = 'X';
								colors[23][75] = 'g'; grid[23][75] = 'X';
								colors[23][76] = 'g'; grid[23][76] = 'X';
								colors[23][77] = 'g'; grid[23][77] = 'X';
								Salle_2_Porte2 = true;    // porte affichée-fermé pour le passage
							}
							else
							{
								colors[23][73] = ' '; grid[23][73] = ' ';
								colors[23][74] = ' '; grid[23][74] = ' ';
								colors[23][75] = ' '; grid[23][75] = ' ';
								colors[23][76] = ' '; grid[23][76] = ' ';
								colors[23][77] = ' '; grid[23][77] = ' ';
								Salle_2_Porte2 = false;   // porte supprimée-ouvert pour le passage
							}
						}
						if (y == 26 && x == 122)  // Salle_2_Switch2  (horizontale)
						{
							if (!Salle_2_Porte3)
							{
								colors[23][131] = 'g'; grid[23][131] = 'X';
								colors[23][132] = 'g'; grid[23][132] = 'X';
								colors[23][133] = 'g'; grid[23][133] = 'X';
								colors[23][134] = 'g'; grid[23][134] = 'X';
								colors[23][135] = 'g'; grid[23][135] = 'X';
								Salle_2_Porte3 = true;    // porte affichée-fermé pour le passage
							}
							else
							{
								colors[23][131] = ' '; grid[23][131] = ' ';
								colors[23][132] = ' '; grid[23][132] = ' ';
								colors[23][133] = ' '; grid[23][133] = ' ';
								colors[23][134] = ' '; grid[23][134] = ' ';
								colors[23][135] = ' '; grid[23][135] = ' ';
								Salle_2_Porte3 = false;   // porte supprimée-ouvert pour le passage
							}
						}
						if (y == 37 && x == 39)  // Salle_1_Switch3  (1 position, permettant d'atteindre le tunnel)
						{
							if (!Salle_1_Porte3)
							{
								colors[41][39] = 'g'; grid[41][39] = 'X';
								Salle_1_Porte3 = true;    // porte affichée-fermé pour le passage
							}
							else
							{
								colors[41][39] = ' '; grid[41][39] = ' ';
								Salle_1_Porte3 = false;   // porte supprimée-ouvert pour le passage
							};
						}
						if (y == 43 && x == 155)  // Salle_2 Switch du computer et objectif final
						{
							beep();
							string message1_key = "";
							string message2_key = "";

							if (!Salle_2_Stockroom_Key)
							{
								colors[34][143] = 'R'; grid[34][143] = ' ';
								colors[34][144] = 'R'; grid[34][144] = 'N';
								colors[34][145] = 'R'; grid[34][145] = 'O';
								colors[34][146] = 'R'; grid[34][146] = ' ';
								colors[34][147] = 'R'; grid[34][147] = 'K';
								colors[34][148] = 'R'; grid[34][148] = 'E';
								colors[34][149] = 'R'; grid[34][149] = 'Y';
								colors[34][150] = 'R'; grid[34][150] = ' ';
							}
							else
							{
								
								//           = "            ";
								message1_key = "    YOU     ";
								message2_key = "    WIN     ";
								for (i = 0; i < message1_key.size(); i++)
								{
									grid[34][141 + i] = message1_key[i];
									colors[34][141 + i] = 'R';
									grid[35][141 + i] = message2_key[i];
									colors[35][141 + i] = 'R';
								}
								victory = true;
							}
						}
					} break;		
		}


		// Affichage coordonnées du joueur
		if (Debug_Mode) 
		{
			// contruction du message_action que peut prendre le joueur
			message_action1 = "y=" + to_string(y) + " x=" + to_string(x) + " ";
			// affichage du message_action que peut prendre le joueur
			i = 0;
			for (i = 0; i < message_action1.size(); i++)
			{
				grid[47][149 + i] = message_action1[i];
			}
		}

	} while (!quit && life > 0);   


	if (quit or life == 0) 
	{ 
		std::cout << "  BYE!  CIAO!  ADIOS AMIGO!" << std::endl; 
	}
	else if (victory)
	{
		std::cout << "VICTOIRE!" << std::endl;
	}

}
