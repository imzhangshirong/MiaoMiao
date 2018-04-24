package dbuntil

import(
	//"fmt"
	"strconv"
	"database/sql"
	"reflect"

	_ "github.com/go-sql-driver/mysql"
)

type DBUntil struct{
	Error error
	Conn *sql.DB
}

func init(){

}

func (p *DBUntil) Init()(*DBUntil){
	return p
}

func (p *DBUntil) Open(host string,port int,user string,password string,database string,charset string) error{
	con:=user+":"+password
	con+="@tcp("+host+":"+strconv.Itoa(port)+")"
	con+="/"+database
	con+="?charset="+charset
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

func Convert(str string,tarType reflect.Type) interface{}{
	dtype:=tarType.Name()
	var re interface{} = str
	switch dtype {
	case "int":
		re,_ = strconv.Atoi(str)
	case "int8":
		re,_ = strconv.ParseInt(str,10,8)
	case "int16":
		re,_ = strconv.ParseInt(str,10,16)
	case "int32":
		re,_ = strconv.ParseInt(str,10,32)
	case "int64":
		re,_ = strconv.ParseInt(str,10,64)
	case "float32":
		re,_ = strconv.ParseFloat(str,32)
	case "float64":
		re,_ = strconv.ParseFloat(str,64)
	case "bool":
		re,_ = strconv.ParseBool(str)
	}
	return re
}

func (p *DBUntil) Quary(quary string,dataType interface{}, args ...interface{})([]interface{},error){
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
						entry.Field(i).Set(reflect.ValueOf(Convert(values[i],entry.Field(i).Type())))
					}
					result = append(result,entry.Interface())
				}
				p.Error = nil
			}
		}
	}
	return result,p.Error
}



func (p *DBUntil) Exec(quary string,args ...interface{}){
	//p.Conn.Exec()
}