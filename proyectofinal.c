
// Fecha de publicación: 23/05/2024
// Hora: 4:20 PM
// Versión del código: V.7.3.0
// Autor: Ing. Andrés Eduardo Álvarez Echeverry
// Nombre del lenguaje utlizado: C
// Versión del lenguaje utilizado: C18
// Versión del compilador utilizado: 6.3.0 (MinGW)
// Versión del S.0 probado: Windows 11 Pro
// Presentado a: Doctor Ricardo Moreno Laverde
// Universidad: Universidad Tecnológica de Pereira
// Programa: Ingeniería de Sistemas y Computación
// Asignatura IS284 Programación II
// Descripción: Aplicativo que grafica funciones polinomicas
// Salvedad: 1. Cuando se pida la funcion polinomica el programa solo funcionará correctamente 
//              si no colocamos espacios entre la funcion, por ejemplo su uso correcto será: 
//              "3x+3" o "4x-9", si se usa con espacios no se garantiza el correcto funcionamiento
//              de la aplicación
//           2. Cuando se pone una constante la aplicación no graficará


#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include <conio.h>
#include <windows.h>


void AllUsersLogIn();            // Imprime todos los accesos de los usuarios 
void ClearInputBuffer();         // Limpia el buffer del teclado
void ChangeRootPassword();       // Cambia la contraseña del root
void ChangeUserPassword();       // Cambia la contraseña de un usuario
int CalculateFunction(char[],int, int, int); // Calcula la funcion
int CharToInteger(char user[]);  // Convierte el nombre de usuario a un numero
void CreateFunction(char [] ,int); // hace los llamados para crear la funcion
void DeleteGraphic(char *,int);  // Elimina la grafica de un usuario
void DeleteUser();               // Elimina un usuario
void encrypt(char *);            // Encripta la contraseña dada por el usuario
int FindData(int,int);           // Devuelve el id del usuario que se requiere
void GetMaskedPassword(char *);  // Obtiene la contraseña del usuario y la enmascara
void GetUserNames();             // Obtiene el nombre de usuario 
void GetSign (char * ,char * );  // Obtiene el signo
int IsValidInteger(int , int, int ); // Verifica si el numero es valido
void LogIn(int);                 // Funcion para la entrada de usuarios a la aplicacion
void LogInOrder(int option);     // Ordena los accesos de los usuarios
int Monomial(char [], int);      // Calcula el monomio
void RootMenu();                 // Menu root de la aplicacion
void Order();                    // Ordena los archivos
void PrintGraphicById(int);      // Imprime la grafica de un usuario por referencia
int Polynomial (char [], int );  // Calcula el polinomio
void PrintGraph(int[], int[], int); // Imprime la grafica de un usuario
int RegisterNumber();            // Funcion que indica cuantos registros hay
void Register();                 // Funcion para registrar nuevos usuarios
void Start(int);                 // Funcion para iniciar la aplicacion
void UsersLogIn(char user[]);    // Imprime los accesos de un solo usuario
void UserMenu(char user[],int);  // Menu de usuario



// Estructura para almacenar las fechas
typedef struct {
  short year;
  char month;
  char day;
} date;

// Estructura para almacenar los datos de los graficos
typedef struct {
  char id;
  char graphic[30];
  char increment;
  char leftRange;
  char rightRange;
  char status;
  int  UserId;
} graphics;

// Estructura para almacenar los logueos del usuario
typedef struct {
  char user[20];
  char id;
  int GraphicId;
  date LogInDate;
}dataLogin;

// Estructura del tipo de dato en el que se registran los usuarios
typedef struct {
  char password[30];
  char id;
  char user[30];
  char status;
  int logIn;
} dataRegister;


// Estructura donde se almacenan los datos de los usuarios
typedef struct {
  int userKey;
  char id;
  char status;
} dataRegisterOrder;


typedef struct {
  char displacement;
  char keyWord[25];

} securityStruct; // Estructura que almacena la palabra clave y el
                  // desplazamiento
//-------------------------------------------------------------------------------------

void AllUsersLogIn()
{  
  // Ordeno el archivo de los usuarios por fecha de forma creciente
  LogInOrder(1);

  // Se crean los tipos de datos que se van a usar
  dataLogin data;
  dataRegister dataRegister;
  graphics dataGraphics; 

  // Se definen la cantidad de usuarios que hay
  int limit = RegisterNumber(), id = 0;
  int flag = 0;
  // Se abren los archivos correspondientes
  FILE *file = fopen ("registro.txt", "rb"); 
  FILE *logInFile = fopen ("login.txt" , "rb");
  FILE *graphicsFile = fopen ("graphics.txt", "rb");

  // Salto una estructura en el archivo registro para no 
  // tomar al registro de root
  fseek(file, sizeof (dataRegister), SEEK_SET);
  
  // Se leen los registros del archivo registro para su comparacion
  for (int i = 1; i < limit; i++){
    flag = 0;

    // Leo desde el segundo registro en adelante 
    fread(&dataRegister, sizeof(dataRegister), 1, file);

    // Posiciono el puntero del archivo login en el primer registro
    fseek(logInFile,0,SEEK_SET);

    // Verfico que el usuario no este borrado
    if (dataRegister.status == 0){

      printf ("\nUsername: %s\n"
              "Fecha       Nro grafico   Ecuacion-F(x)\n"
              "-----------------------------------------------\n", dataRegister.user);
      
      while (flag < dataRegister.logIn){

        fread(&data, sizeof(dataLogin), 1, logInFile);
        

        if (0 == strcmp(data.user,dataRegister.user)){
          id = data.GraphicId;
          fseek(graphicsFile, sizeof(graphics) * (id - 1), SEEK_SET);
          fread(&dataGraphics, sizeof(graphics), 1, graphicsFile);

      
          printf ("%i/%i/%i       %i          %s\n", data.LogInDate.day,data.LogInDate.month,
            data.LogInDate.year, data.GraphicId, dataGraphics.graphic);
          flag += 1;
          
        }
        
      }
      printf("-----------------------------------------------\n");
      printf("%i Registros\n",dataRegister.logIn);
    }
  }
  printf("\nPresione Enter para cerrar la aplicacion...");
  fflush(stdout); // Asegura que la salida se muestra en la consola antes de esperar entrada
  ClearInputBuffer(); // Limpia el buffer del teclado
  getchar(); // Espera a que se presione Enter
  system("cls"); // Limpia la pantalla

  fclose (logInFile);
  fclose(file);
  
}

//-------------------------------------------------------------------------------

void ClearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

//--------------------------------------------------------------------------------
void ChangeRootPassword() {
  dataRegister data;

  FILE *file = fopen("registro.txt", "r+");

  fread(&data, sizeof(dataRegister), 1, file);

  char Password[30] = "a";

printf("Aplicativo - Graficador - UTP\n\n"
       "Ingresa tu anterior ");
  GetMaskedPassword(Password);


  if (strcmp(Password, data.password) == 0) {

    printf("Ingresa tu nueva ");
    GetMaskedPassword(data.password);

    printf("Vuelve a ingresar tu nueva ");
    GetMaskedPassword(Password);

    system("cls");

    if (strcmp(Password, data.password) == 0) {
      fseek(file, 0, SEEK_SET);
      fwrite(&data, sizeof(dataRegister), 1, file);
    } else {
      printf("Contrasena incorrecta.\n");
      Sleep(1500);
      system("cls");
      RootMenu();
    } // Fin del if (strcmp(Password,data.password) == 0)
  } else {
    printf("Contrasena incorrecta.\n");
    Sleep(1500);
    system("cls");
    RootMenu();
  } // Fin del if (strcmp(Password,data.password) == 0)

  printf("Contrasena guardada!");
  Sleep(1500);
  system("cls");

  return;
}
//------------------------------------------------------------------------------

void ChangeUserPassword()
{
  char user[30] = "a";

  printf("Aplicativo - Graficador - UTP\n"
         "Entre el usuario: ");
  scanf ("%s", user);

  int id = FindData(CharToInteger(user),0);

  if (0 == strcmp(user, "root")){
      printf ("\nNo se puede cambiar la contrasena del usuario root. \n");
      Sleep(1500);
      system("cls");
      return;
  }

  if (id == 0){
    printf ("El usuario no existe.\n");
    Sleep(1500);
    system("cls");
    RootMenu();
  }else {
      FILE * file = fopen ("registro.txt", "r+");

      dataRegister data;

      char Password[30] = "a";

      fseek(file,sizeof (dataRegister) * (id - 1),SEEK_SET);
      fread (&data, sizeof (dataRegister), 1, file);

      GetMaskedPassword(Password);

      if (strcmp(Password,data.password) == 0){

        printf ("Ingresa tu nueva ");

        GetMaskedPassword(data.password);

        printf ("Ingresa otra vez tu nueva ");

        GetMaskedPassword(Password);

        system("cls");

        if (strcmp(Password, data.password) == 0){
            fseek (file,- sizeof (dataRegister), SEEK_CUR);
            fwrite (&data, sizeof (dataRegister), 1, file);
            printf("Contrasena guardada!");
            Sleep(1500);
            system("cls");

        }else {
          printf ("Contrasena incorrecta.");
          Sleep(1500);
          system("cls");
          RootMenu();
        }
      }else {
        printf ("La contrasena no es correcta.\n");
        Sleep(1500);
        system("cls");
        RootMenu();
      }
      }
  }
//-----------------------------------------------------------------------------

int CalculateFunction(char polino[30], int min, int max, int inc) {
    int numPoints = (max - min) / inc + 1;
    int xValues[numPoints];
    int yValues[numPoints];
    int index = 0;

    for (int i = min; i <= max; i += inc) {
        int y = Polynomial(polino, i);
        printf("x = %i Y = %i\n", i, y);
        xValues[index] = i;
        yValues[index] = y;
        index++;
    }

    PrintGraph(xValues, yValues, numPoints);
    return 1001; 
}

//--------------------------------------------------------------------------------------

int CharToInteger(char user[20]) {
  int number = 0;

  for (unsigned int i = 0; i < strlen(user); i++)
    number += user[i] + i;

  return number;
}

//-------------------------------------------------------------------------

void CreateFunction(char *user, int id)
{
  // Tomo las funcion y sus parametros que posteriormente se guardaran 
  // en el archivo llamado graphics para su busqueda ordemiento y grafica
  char function[30] = "a";
  int min, max , inc;
  printf ("Aplicativo - Graficador - UTP\n\n"
          "Ingresa la funcion polinomica que deseas ver: ");
  scanf("%s", function);

  printf ("Ingresa rango inicial : ");
  scanf("%i",&min);

  printf ("Ingresa rango final : ");
  scanf("%i",&max);

  printf ("Ingresa el incremento : ");
  scanf("%i", &inc);

  system("cls");

  if (min >= max){ // Compruebo que el rango sea correcto
    printf ("El rango inicial debe ser menor que el rango final");
    Sleep(1500);
    system("cls");
    CreateFunction(user,id); // Al no cumplir volvemos a la misma funcion
  }else if (inc <= 0){ // Otra comprobacion del los valores de los rangos
    printf ("El incremento debe ser mayor que 0");

    CreateFunction(user,id);
  }else {

    // Abro los archivos donde se registra la funcion y el acceso a la 
    // aplicacion respectivamente
    FILE * fileGraphic = fopen ("graphics.txt", "ab");
    FILE * file = fopen ("login.txt", "ab");
    FILE * RegisterFile = fopen ("registro.txt", "r+"); 

    // Abro el tipo de dato de cada uno de ellos 
    dataLogin dataLogin;
    graphics data;
    dataRegister dataRegister;

    // Copio el usuario para su posterior registro
    strcpy(dataLogin.user,user);

    // Esto es para almacenar la fecha en la que se hizo el grafico
    time_t fecha;

    time(&fecha);

    struct tm *st = localtime(&fecha);
    dataLogin.LogInDate.year = st->tm_year + 1900;
    dataLogin.LogInDate.month = st->tm_mon + 1;
    dataLogin.LogInDate.day = st->tm_mday;    


    // Tomo la cantidad de estructuras en el archivo graphics.txt
    // para asignarle un valor unico de id que tambien hace referencia 
    // a su posicion en el archivo
    fseek (fileGraphic, 0, SEEK_END);
    
    int structsNumber = ftell(fileGraphic) / sizeof (graphics);
    
    data.id = structsNumber + 1;

    dataLogin.GraphicId = data.id;

    // Coloco los valores de los parametros de la funcion en el archivo
    strcpy(data.graphic,function);

    data.leftRange = min;
    data.rightRange = max;
    data.increment = inc;
    data.status = 0;
    data.UserId = id;

    // Aumento el contador de logueos del usuario en el registro

    fseek(RegisterFile,sizeof(dataRegister) * (id - 1), SEEK_SET);
    fread (&dataRegister,sizeof(dataRegister), 1, RegisterFile);

    fseek(RegisterFile,- sizeof(dataRegister), SEEK_CUR);
    dataRegister.logIn += 1;

    fwrite(&dataRegister, sizeof(dataRegister), 1, RegisterFile);
    
    // Escribo los datos en el archivo
    fwrite (&data, sizeof (graphics), 1, fileGraphic);
    fwrite (&dataLogin,sizeof(dataLogin),1,file);

    // Cierro los archivos
    fclose (file);
    fclose(fileGraphic);
    fclose (RegisterFile);

    CalculateFunction(function,min, max,inc);
  }
}
//---------------------------------------------------------------------------------

void DeleteGraphic(char * user,int userId) {
  int id = 0;

  printf("Aplicativo - Graficador - UTP\n"
         "Ingresa la referencia del grafico que deseas eliminar: ");
  scanf ("%i", &id);
  system("cls");

      if (id == 0){
        printf ("El grafico no existe.\n");
        Sleep(1500);
        system("cls");
        UserMenu(user,id);
      }else{
        graphics dataGr;

        FILE *graphicsFile = fopen("graphics.txt", "rb");

        fseek(graphicsFile, sizeof(graphics) * (id - 1), SEEK_SET);
        
        fread(&dataGr,sizeof(graphics),1,graphicsFile);

        if (dataGr.UserId == userId){

          fseek(graphicsFile, - sizeof(graphics), SEEK_CUR);
          dataGr.status = 1;
          fwrite(&dataGr, sizeof(graphics), 1, graphicsFile);
          

          printf("\nSe ha borrado el grafico");
          Sleep(1700);
          system("cls");

        }else {
          printf ("El grafico no pertenece a este usuario.\n");
          Sleep(1500);
          system("cls");
          UserMenu(user,id);
        }
        
        fclose(graphicsFile);
        UserMenu(user, id);
      }
}


//----------------------------------------------------------------------
void DeleteUser() {
  char user [10] = "a";
  
  printf("Aplicativo - Graficador - UTP\n"
         "Menu para usuario: root Borrar usuario\n"
         "Entre el usuario:\n"
         "Username: ");
  scanf ("%s", user);

  if (0 == strcmp(user, "root")){
    printf ("\nNo se puede eliminar el usuario root. \n");
    Sleep(1500);
    system("cls");
    RootMenu();
  }else {
    int id = FindData(CharToInteger(user),0);
      if (id == 0){
        printf ("El usuario no existe.\n");
        Sleep(1500);
        system("cls");
        RootMenu();
      }else{
        dataRegister data;

        FILE *file = fopen("registro.txt", "r+");

        FindData(CharToInteger(user),1);
        fseek(file, sizeof(dataRegister) * (id - 1), SEEK_SET);
        fread(&data, sizeof(dataRegister), 1, file);

        data.status = 1;

        fseek(file, - sizeof(dataRegister), SEEK_CUR);
        fwrite(&data,sizeof(dataRegister),1,file);
        printf ("Usuario borrado.");
        Sleep(1500);
        system("cls");
        fclose(file);
      }// Fin del if (id == 0)
  }// Fin del if (0 == strcmp (user, "root")){

}
//--------------------------------------------------------------------------------------
void encrypt(char * password) {

  FILE *file = fopen("seguridad.gra", "rb");

  char keyWord[21] = " ";
  char displacement = ' ';

  fread(&keyWord, 20, 1, file);
  displacement = fgetc(file);

  for (unsigned int i = 0; i < strlen( password); i++)
    for (unsigned int j = 0; j < strlen(keyWord); j++)
      if ( password[i] == keyWord[j])
         password[i] = j + displacement;
      else
        password[i] += displacement;
  fclose(file);
  return;
}

//--------------------------------------------------------------------------------------

int FindData(int objective, int deleteUser) { // Se encarga de buscar por busqueda binaria

  Order();
  FILE *file = fopen("registroOrden.txt", "r+");

  dataRegisterOrder index;
  int structSize = sizeof(dataRegisterOrder);

  int begin = 0;
  int end = RegisterNumber();

  while (begin <= end) {
    int mid = begin + (end - begin) / 2;

    fseek(file, structSize * mid, SEEK_SET);
    fread(&index, structSize, 1, file);

    if (objective == index.userKey) {
      if (deleteUser == 0)
        return index.id;
      else{
        fseek(file, - structSize, SEEK_CUR);
        index.userKey = 0;
        fwrite(&index, structSize, 1, file);
        fclose (file);
        return 0;
      }

    }
    // Si el elemento objetivo es menor, buscar en la mitad izquierda
    else if (index.userKey > objective)
      end = mid - 1;

    // Si el elemento objetivo es mayor, buscar en la mitad derecha
    else
      begin = mid + 1;
  }

  fclose(file);

  return 0;
}


//------------------------------------------------------------------------
void GetMaskedPassword(char *password) {
    printf("Password: ");
    char ch;
    int index = 0;

    while ((ch = getch()) != '\r') { // '\r' es el código ASCII para Enter
        if (ch == 8) { // Si se presiona Backspace
            if (index > 0) {
                index--;
                printf("\b \b"); // Mueve el cursor hacia atrás, imprime un espacio y luego mueve el cursor hacia atrás nuevamente
            }
        } else if (ch != 13 && index < 30) { // Si no se presiona Enter y no se excede la longitud de la contraseña
            password[index++] = ch;
            printf("*");
        }
    }
    password[index] = '\0'; // Añade el terminador nulo al final de la cadena
    printf("\n");

    // Comprueba si la longitud de la contraseña es válida
    if (strlen(password) < 3 || strlen(password) > 28) {
        printf("La contrasena debe tener entre 3 y 28 caracteres.\n");
        GetMaskedPassword(password); // Llama recursivamente a la función si la longitud no es válida
    } else {
        encrypt(password); // Encripta la contraseña
    }
}

//--------------------------------------------------------------------------

void GetUserNames(){
  char user[30] = "a";
  
  printf("Aplicativo - Graficador - UTP\n"
         "Listados de acceso por usuario\n"
         "Entre el usuario:\n"
         "Username: ");
  scanf ("%s", user);

  UsersLogIn(user);
  
}
//--------------------------------------------------------------------------

void GetSign (char * sign,char * poly)
{
  for (unsigned int i = 0, j = 1; i < strlen(poly); i ++)
   if (poly[i] == '+' || poly[ i] == '-'){
   sign[j] = poly[i];
   j ++;
    }

}

//---------------------------------------------------------------------------------------
int IsValidInteger(int number, int min, int max)
{
  if (number < min || number > max)
    return 1;
  else
    return 0;
}



//-------------------------------------------------------------------------
void LogIn(int counter) {
  if (counter <= 3) {

    char user[20] = "a", password[20] = "a";
    printf("Aplicativo - Graficador - UTP\n\n"
           "Username: ");
    scanf("%s", user);

    int id = FindData(CharToInteger(user),0);

    if (id == 0) {
      printf("El usuario no existe. \n");
      Start(1500);
      system("cls");
      LogIn(counter + 1);
    } else {

      dataRegister data;
      FILE *file = fopen("registro.txt", "r+");

      fseek(file, sizeof(dataRegister) * (id - 1), SEEK_SET);
      fread(&data, sizeof(dataRegister), 1, file);

      GetMaskedPassword(password);

      system("cls");

      

      if (0 == strcmp(password, data.password)) {
        printf("\nLogeo exitoso!\n");
        Sleep(1500);
        system("cls");

          if (strcmp (user, "root") == 0)
        RootMenu();

          else {
            UserMenu(user,id);

          }
      } else {
        printf("La contrasena no coincide.\n");
        Sleep(1500);
        system("cls");
        LogIn(counter + 1);

      }
    }
  } else

    return;
}
//-----------------------------------------------------------------------------

void LogInOrder(int option){
    dataLogin data1, data2, aux;

    int structSize = sizeof(dataLogin);

   FILE *fileLogIn = fopen("login.txt", "r+"); // Abre el archivo en un formato editable

    fseek(fileLogIn, 0, SEEK_END); // Se ubica al final del archivo

      int structNumber = ftell(fileLogIn) / structSize; // Obtiene el numero de registros en el archivo

    if (structNumber > 1){
    // en el archivo
    fseek(fileLogIn, 0, SEEK_SET);

    // Ordena el archivo de menor a mayor por el algoritmo de burbuja
    for (int i = 1; i <= structNumber; i++) {
      for (int j = 0; j <= structNumber - 1; j++) {

        fseek(fileLogIn, structSize * j, SEEK_SET);
        fread(&data1, structSize, 1, fileLogIn);
        fread(&data2, structSize, 1, fileLogIn);

        if (option == 1){ 

        if ((data1.LogInDate.year + data1.LogInDate.month + data1.LogInDate.day)           > (data2.LogInDate.year + data2.LogInDate.month + data2.LogInDate.day)) {

          aux = data1;
          data1 = data2;
          data2 = aux;

          fseek(fileLogIn, -structSize * 2, SEEK_CUR);
          fwrite(&data1, structSize, 1, fileLogIn);
          fwrite(&data2, structSize, 1, fileLogIn);
          fclose(fileLogIn);
        }
        }else {

          if ((data1.LogInDate.year + data1.LogInDate.month + data1.LogInDate.day) < (data2.LogInDate.year + data2.LogInDate.month + data2.LogInDate.day)){

          aux = data1;
          data1 = data2;
          data2 = aux;

          fseek(fileLogIn, -structSize * 2, SEEK_CUR);
          fwrite(&data1, structSize, 1, fileLogIn);
          fwrite(&data2, structSize, 1, fileLogIn);
          fclose(fileLogIn);
        }
        }
      }
    }
    }

  }



//--------------------------------------------------------------------------------------

int Monomial(char mon[10], int x) {

    char originalString[10] = "a";

    strcpy(originalString,mon);

    char *takeNum;

    // Extraer el coeficiente
    takeNum = strtok(mon, "x");
    int coefficient = atoi(takeNum);

    if (strcmp (originalString,mon) != 0) {

    if (coefficient == 0)
      coefficient = 1;

    // Extraer el exponente
    takeNum = strtok(NULL, "\0");
    int exp = atoi(takeNum);

    if (exp == 0)
    exp = 1;


    return coefficient * pow(x, exp);
    }
    else 
    return atoi (originalString);
}

//-----------------------------------------------------------------------

void RootMenu() {
  printf("\nAplicativo - Graficador - UTP\n"
         "Menu principal para usuario: root\n\n"
         "0.  Salir del aplicativo\n"
         "1.  Crear nuevo usuario\n"
         "2.  Borrar usuario\n"
         "3.  Listar accesos por usuario\n"
         "4.  Listar accesos de todos los usuarios\n"
         "5.  Imprimir grafico por referencia\n"
         "6.  Cambiar password del usuario root\n"
         "7.  Cambiar password de un usuario\n"
         "-   Ingrese su opcion: ");

  int option = 0;
  scanf("%i", &option);

  system("cls");

  if (IsValidInteger(option, 0, 7) == 0) {
  switch (option) {
  case 0:

    printf("\nAplicacion finalizada.");
    
    Sleep(1500);
    system("cls");

    return;

    break;
  case 1:
    Register();
    break;
  case 2:
    DeleteUser();
    break;
  case 3:
    GetUserNames();
    break;
  case 4:
    AllUsersLogIn();
    break;
  case 5:
    PrintGraphicById(0);  
    break;
  case 6:
    ChangeRootPassword();
    break;
  case 7:
    ChangeUserPassword();
    break;
  default:
    printf("Opcion no valida.");
    RootMenu();
  }
  }else
    RootMenu();
}

//--------------------------------------------------------------------------------------

void Order() { // Ordena los archivos
  dataRegisterOrder data1, data2, aux;

  int structSize = sizeof(dataRegisterOrder),

      structNumber = RegisterNumber(); // Guarda la cantidad de estructuras

  if (structNumber > 1){
  // En el archivo
  FILE *fileOrder = fopen("registroOrden.txt", "r+"); // Abre el archivo en un formato editable
  fseek(fileOrder, 0, SEEK_SET);

  // Ordena el archivo de menor a mayor por el algoritmo de burbuja
  for (int i = 1; i <= structNumber; i++) {
    for (int j = 0; j <= structNumber - 1; j++) {

      fseek(fileOrder, structSize * j, SEEK_SET);
      fread(&data1, structSize, 1, fileOrder);
      fread(&data2, structSize, 1, fileOrder);

      if (data1.userKey > data2.userKey) {

        aux = data1;
        data1 = data2;
        data2 = aux;

        fseek(fileOrder, - structSize * 2, SEEK_CUR);
        fwrite(&data1, structSize, 1, fileOrder);
        fwrite(&data2, structSize, 1, fileOrder);
        fclose(fileOrder);
      }
    }
  }
  }

}

//-------------------------------------------------------------------------

void PrintGraphicById(int id)
{
  FILE *file = fopen("registro.txt", "rb");
  FILE *graphicsFile = fopen("graphics.txt", "rb");

  int idGr = 0;
  dataRegister data;
  graphics dataGr;

  fseek(file, sizeof(dataRegister)*(id-1), SEEK_SET);
  fread(&data, sizeof(dataRegister), 1, file);
  fseek(file, 0, SEEK_SET);
  
  if (id == 0){
    printf ("Aplicativo - Graficador - UTP\n"
            "Impresion de grafica por referencia\n"
            "Entre Nro (Referencia): ");
    scanf("%i", &id);
    printf ("Username: %s\n",data.user);
    
    fseek(graphicsFile,sizeof (graphics) * (id - 1), SEEK_SET);
    fread(&dataGr,sizeof(graphics),1,graphicsFile);
    
    if (dataGr.status == 0){
      printf ("f(x) = %s  Rango: %i a %i Incremento: %i\n\n",dataGr.graphic, dataGr.leftRange,dataGr.rightRange,dataGr.increment);
      CalculateFunction(dataGr.graphic, dataGr.leftRange,dataGr.rightRange,dataGr.increment);
      
    }else{
      printf("El grafico no existe");
      Start(1500);
      system("cls");
      RootMenu();
    }
      
  }else {

    printf ("Ingresa la referencia de tu grafico: ");
    scanf("%i", &idGr);
    
    fseek(graphicsFile,sizeof (graphics) * (idGr - 1), SEEK_SET);
    fseek(file , sizeof(dataRegister) * (id - 1),SEEK_SET);
  
    fread(&dataGr,sizeof(graphics),1,graphicsFile);
    fread(&data,sizeof (dataRegister), 1, file); 

    if (dataGr.UserId == data.id)
    {

      if (dataGr.status == 0){
        CalculateFunction(dataGr.graphic,dataGr.leftRange,dataGr.rightRange,dataGr.increment);
      }else
      {
       printf("El grafico no existe");
       Sleep(1500);
       system("cls");
       UserMenu("0", id);
      }
    }else 
    {
      printf("El grafico no es parte de este usuario");
      Sleep(1500);
      system("cls");
      UserMenu("0", id);
    }
  }
}
//-----------------------------------------------------------------------
int Polynomial (char poly[30], int x){
  char sign[10] = "+",mon[10] = "a";
  int j = 0,result = 0,counter = 0;

  GetSign(sign,poly);

  for (unsigned int i = 0; i <= strlen(poly); i ++, j++)

    if (poly[i] == '+' || poly[ i] == '-' || poly[ i] == '\0'){
      mon[j] = '\0';

      if (sign[counter] == '+'){
        result += Monomial(mon,x);
        counter ++;
      }else {
        result -= Monomial(mon,x);
        counter ++;
      }    
      j = -1;

    }else 
      mon[j] = poly[ i];

  return result;

}
//-------------------------------------------------------------------------
void PrintGraph(int x[], int y[], int n) {
    // Define el tamaño del gráfico
    int graphHeight = 25;
    int graphWidth = 80;
    char graph[graphHeight][graphWidth];

    // Encuentra los valores máximos y mínimos de x e y
    int xMin = x[0], xMax = x[0], yMin = y[0], yMax = y[0];
    for (int i = 1; i < n; i++) {
        if (x[i] < xMin) xMin = x[i];
        if (x[i] > xMax) xMax = x[i];
        if (y[i] < yMin) yMin = y[i];
        if (y[i] > yMax) yMax = y[i];
    }

    // Inicializa el gráfico con espacios
    for (int i = 0; i < graphHeight; i++) {
        for (int j = 0; j < graphWidth; j++) {
            graph[i][j] = ' ';
        }
    }

    // Marca los ejes X e Y
    int xAxis = graphHeight - 1 - (0 - yMin) * (graphHeight - 1) / (yMax - yMin);
    int yAxis = (0 - xMin) * (graphWidth - 1) / (xMax - xMin);
    if (xAxis >= 0 && xAxis < graphHeight) {
        for (int j = 0; j < graphWidth; j++) {
            graph[xAxis][j] = '-';
        }
    }
    if (yAxis >= 0 && yAxis < graphWidth) {
        for (int i = 0; i < graphHeight; i++) {
            graph[i][yAxis] = '|';
        }
    }
    if (xAxis >= 0 && xAxis < graphHeight && yAxis >= 0 && yAxis < graphWidth) {
        graph[xAxis][yAxis] = '+';
    }

    // Marca los puntos en el gráfico
    for (int i = 0; i < n; i++) {
        int xPos = (x[i] - xMin) * (graphWidth - 1) / (xMax - xMin);
        int yPos = graphHeight - 1 - (y[i] - yMin) * (graphHeight - 1) / (yMax - yMin);
        if (xPos >= 0 && xPos < graphWidth && yPos >= 0 && yPos < graphHeight) {
            graph[yPos][xPos] = '*';
        }
    }

    // Imprime el gráfico
    for (int i = 0; i < graphHeight; i++) {
        for (int j = 0; j < graphWidth; j++) {
            printf("%c", graph[i][j]);
        }
        printf("\n");
    }
    printf("\nPresione Enter para cerrar la aplicacion...");
    fflush(stdout); // Asegura que la salida se muestra en la consola antes de esperar entrada
    ClearInputBuffer(); // Limpia el buffer del teclado
    getchar(); // Espera a que se presione Enter
    system("cls"); // Limpia la pantalla
 }
//--------------------------------------------------------------------------------------

int RegisterNumber() { // Retorna la cantidad de estructuras en el archivo

  FILE *file = fopen("registroOrden.txt", "rb");

  int structSize = sizeof(dataRegisterOrder), totalBytes, structNumber;

  fseek(file, 0, SEEK_END);
  totalBytes = ftell(file);
  structNumber = totalBytes / structSize;

  fclose(file);
  return structNumber;
}

//--------------------------------------------------------------------------------------

void Register() {

  dataRegister data;
  dataRegisterOrder dataSec;

  char password[20] = "";


  printf("Aplicativo - Graficador - UTP\n"
         "Menu para usuario: root Crear nuevo usuario\n"
         "Entre el nuevo usuario:\n"
         "Username: ");
  scanf("%s", data.user);


  if (0 != FindData(CharToInteger(data.user),0)) {
    printf("El usuario ya existe, intenta otro nombre de usuario\n\n");

    Register();

  } else {

    GetMaskedPassword(data.password);

    printf("Ingresa de nuevo tu ");

    GetMaskedPassword(password);

    system("cls");

    if (0 == strcmp(password, data.password)) {
      FILE *file = fopen("registro.txt", "ab");
      FILE *fileOrder = fopen("registroOrden.txt", "ab");
      data.status = 0;
      dataSec.id = 0;
      data.logIn = 0;
      data.id = RegisterNumber() + 1;
      dataSec.id = data.id;

      fwrite(&data, sizeof(data), 1, file);
      dataSec.userKey = CharToInteger(data.user);
      fwrite(&dataSec, sizeof(dataSec), 1, fileOrder);

      printf("Contrasena guardada!");

      Sleep(1500);
      system("cls");

    } else
      Register();
  }
}

//--------------------------------------------------------------------------------------
// Registra el primer acceso al aplicativo por parte del usuario root 

void Start(int counter) {

  if (RegisterNumber() == 0) {

    if (counter >= 3)

      return;

    else {
      dataRegister data;

      printf("Aplicativo - Graficador - UTP\nUsername: ");
      scanf("%s", data.user);

      if (0 == strcmp(data.user, "root")) {

        GetMaskedPassword(data.password);

        system("cls");

        FILE *file = fopen("registro.txt", "wb");
        FILE *fileOrder = fopen("registroOrden.txt", "ab");
        dataRegisterOrder dataSec;
        data.status = 0;
        dataSec.status = 0;
        dataSec.id = 1;
        data.id = 1;
        dataSec.userKey = CharToInteger(data.user);

        fwrite(&dataSec, sizeof(dataRegisterOrder), 1, fileOrder);
        fwrite(&data, sizeof(dataRegister), 1, file);
        fclose(file);
        fclose(fileOrder);

        return;

      } else {

        printf("Usuario incorrecto. \n");

        Sleep(1500);
        system("cls");

        Start(counter + 1);

        return;
      }
    } // fin del if (counter >= 3)
  } else
    LogIn(0);
  // fin del if (RegisterNumber() == 0) {
}

//---------------------------------------------------------------------------------------

void UsersLogIn(char user[20])
{
  int option = 0;
  printf("Aplicativo - Graficador - UTP\n"
         "Modulo listar accesos - Usuario: %s\n"
         "Ordenar por fecha.\n(1) Orden ascendente: \n(2) Orden Descendente\nSeleccione su opcion: ",user);
  scanf ("%i", &option);

  system("cls");

  if (IsValidInteger(option, 1, 2) == 0){

  LogInOrder(option);

    dataLogin data;
    dataRegister dataRegister;
    graphics dataGraphics;

    int flag = 0,id = 0;

    FILE *file = fopen ("registro.txt", "rb"); 
    FILE *logInFile = fopen ("login.txt" , "rb");
    FILE *graphicsFile = fopen ("graphics.txt", "rb");

    id = FindData(CharToInteger(user),0);

    if (id != 0) {
    fseek (file,sizeof(dataRegister) * (id - 1),SEEK_SET);

      fread(&dataRegister, sizeof(dataRegister), 1, file);

      if (dataRegister.status == 0){

        printf ("\nFecha       Nro grafico    Ecuacion-F(x)\n"
                "------------------------------------------------------------\n");

        while (flag < dataRegister.logIn){


          fread(&data, sizeof(dataLogin), 1, logInFile);

            if (0 == strcmp(data.user,dataRegister.user)){

            fseek(graphicsFile, sizeof(graphics) * (data.GraphicId - 1), SEEK_SET);

              fread(&dataGraphics, sizeof(graphics), 1, graphicsFile);



              printf ("%i/%i/%i        %i            %s\n", data.LogInDate.day,data.LogInDate.month,
                data.LogInDate.year, data.GraphicId, dataGraphics.graphic);

          flag += 1;
            }
        }
        printf("------------------------------------------------------------\n");
        printf("%i Registros\n", dataRegister.logIn);
        printf("\nPresione Enter para cerrar la aplicacion...");
        fflush(stdout); // Asegura que la salida se muestra en la consola antes de esperar entrada
        ClearInputBuffer(); // Limpia el buffer del teclado
        getchar(); // Espera a que se presione Enter
        system("cls"); // Limpia la pantalla
    }

  fclose (logInFile);
  fclose(file);
    } else {
      printf ("El usuario no existe");
      Sleep(1500);
      system("cls");
      RootMenu();
    }
      
  }else 
    UsersLogIn(user);
}
//----------------------------------------------------------------------------
void UserMenu(char user[20], int id) 
{
  printf("Aplicativo - Graficador - UTP\n"
     "Menu principal para usuario: %s\n\n"
     "0.  Salir del aplicativo\n"
     "1.  Crear nuevo grafico\n"
     "2.  Borrar grafico existente\n"
     "3.  Listar accesos\n"
     "4.  Imprimir grafico por referencia\n"
     "5.  Cambiar password del usuario: %s\n"
     "-   Ingrese su opcion: ",user,user);

  int userOption = 0;
  scanf("%i", &userOption);

if (IsValidInteger(userOption, 0, 7) == 0){

  system("cls");

  switch (userOption) 
  {
  case 0:
  printf("\nAplicacion finalizada.");
  Sleep(1500);
  system("cls");
  return; 
  break;

  case 1:
    CreateFunction(user,id);
  break;

  case 2:
    DeleteGraphic(user,id);
  break;

  case 3:
    UsersLogIn(user);
  break;

  case 4:
    PrintGraphicById(id);
  break;

  case 5:
  ChangeUserPassword(); 
  break;

  default:
  printf("Opcion no valida\n");
  UserMenu(user,id);
  }
}else 
  UserMenu(user,id);
}
//-------------------------------------------------------------------------------

int main() 
{

  Start(0);

  return 0;

}