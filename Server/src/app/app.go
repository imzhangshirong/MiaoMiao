package app

import(
	router "package/router"
	dbuntil "package/dbuntil"
)

var Router *router.Router
var DBUntil *dbuntil.DBUntil

func init(){
	Router = new(router.Router).Init()
	DBUntil = new(dbuntil.DBUntil).Init()
}