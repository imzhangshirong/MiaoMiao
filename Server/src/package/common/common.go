package common

import(
	"io/ioutil"
	"os"
	"strings"
    "path/filepath"
    "os/exec"
    "bytes"
    "reflect"
    "strconv"
	"crypto/md5" 
    "encoding/hex"

	debug "package/debug"
)

func ExecShell(command string)(string,error){
    cmd := exec.Command("/bin/bash", "-c", command)
    var out bytes.Buffer
    cmd.Stdout = &out
    err := cmd.Run()
    return out.String(), err
}

func GetGOPATH() string{
    path,_:= ExecShell("echo $GOPATH")
    return strings.Trim(path,"\n")
}

func GetCurrentExecDir() string {
    dir, err := filepath.Abs(filepath.Dir(os.Args[0]))
    if err != nil {
        debug.Error(err.Error())
    }
    return strings.Replace(dir, "\\", "/", -1)
}

func ListDir(dirPth string, suffix string) (files []string, err error) {
    files = make([]string, 0, 10)
    dir, err := ioutil.ReadDir(dirPth)
    if err != nil {
        return nil, err
    }
	suffix = strings.ToLower(suffix)
	
    for _, fi := range dir {
        if fi.IsDir() { // ignore dir
            continue
        }
        if strings.HasSuffix(strings.ToLower(fi.Name()), suffix) {
            files = append(files, fi.Name())
		}
		
    }
    return files, nil
}

func MapDir(dirPth, suffix string) (files []string, err error) {
    files = make([]string, 0, 30)
    suffix = strings.ToUpper(suffix)
    err = filepath.Walk(dirPth, func(filename string, fi os.FileInfo, err error) error {
        //if err != nil { //ignore
        // return err
        //}
        if fi.IsDir() {
            return nil
        }
        if strings.HasSuffix(strings.ToUpper(fi.Name()), suffix) {
            files = append(files, filename)
        }
        return nil
    })
    return files, err
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

func MD5(data string) string{
    h := md5.New() 
    h.Write([]byte(data))
    cipherStr := h.Sum(nil) 
    return hex.EncodeToString(cipherStr)
}
