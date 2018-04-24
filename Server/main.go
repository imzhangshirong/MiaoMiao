package main

import (
	//"net/http"
	//"strconv"


	app "app"
	config "config"
	debug "package/debug"

	module_login "module/login"
)

type UserData struct{
	Id int
	Username string
}

func main() {
	debug.Info("server running...")
	app.DBUntil.Open(
		config.MySql.Host,
		config.MySql.Port,
		config.MySql.User,
		config.MySql.Password,
		config.MySql.Database,
		config.MySql.Charset)
	userData,err := app.DBUntil.Quary("SELECT * FROM `user` WHERE `id` = ?",UserData{},1)
	if err == nil {
		for _,v := range userData{
			data := v.(UserData)
			debug.Log(data.Username)
		}
	}else{
		debug.Error(err.Error())
	}

	debug.Info("binding router...")
	bindHandle()
	debug.Info("listening...")
	app.Router.Listen(config.Server.Host,config.Server.Port)
}

func bindHandle(){
	app.Router.AddHandle("/login",module_login.Login)
}