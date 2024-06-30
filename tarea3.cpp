#include <iostream>
#include <fstream>
#include <string>
using namespace std;

struct cuenta
{
    // El rol es el identificador de la persona.
    // El nombre y la descripcion son valores asociados al rol
    string rol, nombre, descripcion;
    bool existencia = false; // Si es false, no existe cuenta, y cuando sea creada una, este parametro será cambiado a true
    int colisiones = 0;
};
class registro_cuentas
{
private:
    float factor_de_carga = 0.0;
    cuenta *tabla;    // Aca se almacenaran los elementos de la tabla
    int ranuras = 15; // Cuantas ranuras tiene la tabla hash (inicialmente)
    // Se obtiene el hash dado el rol y Usamos el numero 17 para hashing constante, debido a que es un numero primo y alto
    int hash(string rol)
    {
        int valorh1 = 0;
        for (int i = 0; i < 10; i++)
        {
            if (rol[i] != '-')
            {

                valorh1 = (valorh1 * 17 + rol[i]) % ranuras;
            }
        }
        // cout << "Valor de h1 es: " << valorh1 << endl;
        return valorh1;
    };
    int hash2(string rol)
    {
        int valorh2 = 0;
        for (int i = 0; i < 10; i++)
        {
            if (rol[i] != '-')
            {

                valorh2 = (valorh2 * 13 + rol[i]) % ranuras;
            }
        }
        return valorh2;
    };
    int p(string rol, int i)
    { // Se otiene la ranura a revisar en caso de colisión dado el rol y el intento i

        return (hash(rol) + hash2(rol) * i) % ranuras;
    };

public:
    registro_cuentas(); // (Recuerde que puede crear con distintos parametros)
    ~registro_cuentas();
    cuenta obtener(string rol);                     // Dado el rol, devuelve la cuenta con ese rol
    void agregar(cuenta c);                         // Se agrega una cuenta a la tabla
    void eliminar(string rol);                      // Se elimina la cuenta
    void modificar(string rol, string descripcion); // Se modifica la descripcion del rol
    void redimensionar(int n);                      // Se redimensiona la tabla a n espacios
    void estadisticas();                            // Debe mostrar las estadisticas
    void act_fc();
    float valor_fc();
};

registro_cuentas::registro_cuentas()
{
    tabla = new cuenta[ranuras];
}

registro_cuentas::~registro_cuentas()
{
    delete[] tabla;
}

cuenta registro_cuentas::obtener(string rol)
{
    int pos = hash(rol);
    int col = 0;
    int cont_obtener = 0;

    while (tabla[pos].existencia != false && cont_obtener < ranuras)
    {

        if (tabla[pos].rol == rol)
        {
            return tabla[pos]; // El rol se encuentra en la tabla hash
        }
        col++;
        cont_obtener++;
        pos = p(rol, col); // Se busca nueva posicion utilizando el h2
    }
    cuenta cuenta_inex; // Se crea una cuenta inexistente para definir que no existe el rol buscado
    return cuenta_inex;
}
void registro_cuentas::agregar(cuenta c)
{
    if (obtener(c.rol).existencia == true)
    {
        cout << "Rol ya existente" << endl;
    }
    else
    {
        int pos = hash(c.rol);
        bool celda_tabla = false;

        if (tabla[pos].existencia == false)
        {
            tabla[pos] = c;
            tabla[pos].existencia = true;
        }
        else
        {
            int cont = 1;
            while (celda_tabla != true)
            {
                int nueva_pos = p(c.rol, cont);
                if (tabla[nueva_pos].existencia == false)
                {
                    tabla[nueva_pos] = c;
                    tabla[nueva_pos].existencia = true;
                    tabla[nueva_pos].colisiones = cont;
                    celda_tabla = true;
                }
                else
                {
                    cont++;
                }
            }
        }
    }
}
void registro_cuentas::eliminar(string rol)
{
    cuenta cuenta_a_eliminar = obtener(rol);
    if (cuenta_a_eliminar.existencia == false)
    {
        cout << "Rol no existente" << endl;
    }
    else
    {
        cuenta cuenta_vacia;
        for (int i = 0; i < ranuras; i++)
        {
            if (tabla[i].rol == rol)
            {
                tabla[i] = cuenta_vacia;
            }
        }
    }
}
/*void registro_cuentas::modificar(string rol, string descripcion){

}*/

void registro_cuentas::redimensionar(int n) // Nos fijaremos que si la tabla es mayor a 60&, la aumentaremos para disminuir cantidad de posibles colisiones
{
        int n_ranueras = ranuras * 2;
        cuenta *n_tabla = new cuenta[n_ranueras]; // POR TERMINAR aaa
        for (int i = 0; i < ranuras; i++){
            if (tabla[i].existencia != false){
                int nt_pos = hash(tabla[i].rol);
                int nt_col = 0;
                while (n_tabla[nt_pos].existencia != true){
                    nt_col++;
                    nt_pos = p(tabla[i].rol,nt_col);
                }
                n_tabla[nt_pos] = tabla[i];
            }
        }
        delete[] tabla;
        tabla = n_tabla;
        ranuras = n;
        act_fc();
    }
void registro_cuentas::estadisticas()
{
    float contador_stats = 0;
    for (int i = 0; i < ranuras; i++)
    {
        if (tabla[i].existencia == true)
        {
            contador_stats++;
        }
    }
    cout << "RANURAS OCUPADAS: " << contador_stats << endl;
    cout << "RANURAS TOTALES: " << ranuras << endl;
    cout << "FACTOR DE CARGA: " << factor_de_carga << endl;
}
void registro_cuentas::act_fc()
{
    float contador_fc = 0.0;
    for (int i = 0; i < ranuras; i++)
    {
        if (tabla[i].existencia == true)
        {
            contador_fc++;
        }
    }
    factor_de_carga = contador_fc / ranuras;
}
int main()
{
    ifstream archivo;
    string text_fun, text_rol, text_desc, text_nom;
    registro_cuentas regc;
    archivo.open("pruebas.txt", ios::in);

    if (!archivo.is_open())
    {
        cout << "Error al abrir el archivo\n";
        exit(1); // error abriendo el archivo
    }
    while (archivo >> text_fun)
    {
        cout << "La funcion acutal es: " << text_fun << endl;
        if (text_fun == "AGREGAR")
        {
            archivo >> text_rol;
            archivo >> text_nom;
            archivo >> text_desc;
            cuenta nueva_cuenta;
            nueva_cuenta.nombre = text_nom;
            nueva_cuenta.rol = text_rol;
            nueva_cuenta.descripcion = text_desc;
            regc.agregar(nueva_cuenta);
            regc.act_fc();
        }
        else if (text_fun == "QUITAR")
        {
            archivo >> text_rol;
            regc.eliminar(text_rol);
            regc.act_fc();
        }
        else if (text_fun == "MODIFICAR")
        {
            archivo >> text_rol;
            archivo >> text_desc;
            cout << "ROL: " << text_rol << endl;
            cout << "DESC: " << text_desc << endl;
        }
        else if (text_fun == "OBTENER")
        {
            archivo >> text_rol;
            cuenta cuenta_buscar = regc.obtener(text_rol);
            if (cuenta_buscar.existencia == true){
                cout << cuenta_buscar.nombre << " " << cuenta_buscar.descripcion << endl;
            }
            else{
                cout << "Rol no existente" << endl;
            }
        }
        else if (text_fun == "ESTADISTICAS"){
            regc.estadisticas();
        }

        }
        regc.~registro_cuentas();
        return 0;
    }
    