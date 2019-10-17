^c::
clipboard =  ; 
Send ^x
ClipWait  ; 
clipboard = <font color='red'>%clipboard%</font>
Send ^v
return
