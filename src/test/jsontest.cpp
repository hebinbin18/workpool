#include "jsontest.h"
#include "json/reader.h"
using namespace std;

int encodetest() {
	string test ="{\"id\":1,\"name\":\"kurama\"}";
	 Json::Reader reader;
	 Json::Value value;
	 if(reader.parse(test,value))
	 {
	  if(!value["id"].isNull())
	  {
	   cout<<value["id"].asInt()<<endl;
	   cout<<value["name"].asString()<<endl;
	  }
	 }

	 return 0;
}

int decodetest()
{

	return 0;
}
