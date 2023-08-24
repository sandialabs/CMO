#include <fstream>

using namespace std;

int main()
{
ifstream foo("LKDJF");
if (!foo)
   cerr << "HERE" << endl;
else
   cerr << "THERE" << endl;
char c;
foo >> c;
if (!foo)
   cerr << "HERE" << endl;
else
   cerr << "THERE" << endl;

return 0;
}
