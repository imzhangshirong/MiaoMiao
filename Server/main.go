package main

import (
	//"net/http"
	"strconv"


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
	debug.Info("Server running...")
	//----------------------------------
	debug.Info("Connect To Database...")
	app.DBUtil.Open()
	//----------------------------------
	debug.Info("Import Session...")
	app.SessionManager.ImportSession()
	app.SessionManager.Clean()
	app.SessionManager.AutoClean()
	//----------------------------------
	debug.Info("Binding router...")
	bindHandle()
	//----------------------------------
	debug.Info("Listening... "+config.Server.Host+":"+strconv.Itoa(config.Server.Port))
	app.Router.Listen()
}

func bindHandle(){
	app.Router.AddHandle("/login",module_login.Login)
}