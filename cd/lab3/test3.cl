class B{
	s : string <- "hello";

	g(y : string) : int {
		y.concat(s)
	};

	f(x : int) : int {
		x+1;
	};
};