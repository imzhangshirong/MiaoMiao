package DBUtil

import(
	//"fmt"
	"strconv"
	"database/sql"
	"reflect"

	_ "github.com/go-sql-driver/mysql"

	common "package/common"
)

type DBUtil struct{
	Error error
	Conn *sql.DB
	host string
	port int
	user string
	password string
	database string
	charset string
}

func init(){

}

func (p *DBUtil) Init(host string,port int,user string,password string,database string,charset string)(*DBUtil){
	p.host = host
	p.port = port
	p.user = user
	p.password = password
	p.database = database
	p.charset = charset
	return p
}

func (p *DBUtil) Open() error{
	con:=p.user+":"+p.password
	con+="@tcp("+p.host+":"+strconv.Itoa(p.port)+")"
	con+="/"+p.database
	con+="?charset="+p.charset
	//fmt.Println(con)
	db,err:=sql.Open("mysql",con)
	if err!=nil {
		p.Error = err
	}else{
		p.Conn = db
		p.Error = nil
	}
	return p.Error
}



func (p *DBUtil) Quary(quary string,dataType interface{}, args ...interface{})([]interface{},error){
	var result []interface{}
	if p.Conn!=nil {
		var rows *sql.Rows
		var err error
		if args == nil {
			rows,err=p.Conn.Query(quary)
		}else{
			rows,err=p.Conn.Query(quary,args...)
		}
		if err!=nil {
			p.Error = err
		} else{
			columns,err:= rows.Columns()
			if err!=nil {
				p.Error = err
			} else{
				ccount := len(columns)
				values := make([]string, ccount)
				ptr := make([]interface{}, ccount)
				dtype := reflect.TypeOf(dataType)
				for rows.Next() {
					for i := 0; i < ccount; i++ {
						ptr[i] = &values[i]
					}
					rows.Scan(ptr...)
					entry := reflect.New(dtype).Elem()
					for i:=0;i<ccount && i<entry.NumField();i++ {
						entry.Field(i).Set(reflect.ValueOf(common.Convert(values[i],entry.Field(i).Type())))
					}
					result = append(result,entry.Interface())
				}
				p.Error = nil
			}
		}
	}
	return result,p.Error
}



func (p *DBUtil) Exec(quary string,args ...interface{}){
	//p.Conn.Exec()
}