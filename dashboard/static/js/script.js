function urlTest(params) {
    let currUrl =  window.location.pathname;
    // console.log("ss",currUrl);    
    switch (currUrl) {
        case "/":
            document.getElementById("page_1").classList.add("current-url");
            document.getElementById("page_2").classList.remove("current-url")
            break;
        case "/info":
            document.getElementById("page_2").classList.add("current-url");
            document.getElementById("page_1").classList.remove("current-url")
            break;    
        default:
            break;
    }
}