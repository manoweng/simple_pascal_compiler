var t: integer;
function g(n, r: integer; var k : integer): integer;
function f(i: integer): integer;
var s, t: integer;
begin
s:=i*i;
if r<s
then begin k:=k+1; f:=0 end
else begin f:=g(n-1, r-s, t) + f(i+1);
k:=k+t
end
end ;
begin
if n=0
then begin k:=1; g:=1 end
else begin
k:=0;
g:= g(n-1, r, t) + 2*f(1);
k:=k+t
end
end
begin
write(a)
end