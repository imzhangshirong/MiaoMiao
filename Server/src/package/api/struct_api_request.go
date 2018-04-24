package api

import(
	"net/http"
)

type ApiRequest struct{
	Request *http.Request
}
