package config

import(
	//"fmt"
)

var Error *ErrorConfig
var MySql *MySqlConfig
var Server *ServerConfig
var Session *SessionConfig

func init(){
	Error = new(ErrorConfig).Init()
	MySql = new(MySqlConfig).Init()
	Server = new(ServerConfig).Init()
	Session = new(SessionConfig).Init()
}