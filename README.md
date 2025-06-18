# libCRedirect

libCRedirect is a lightweight C library designed to intercept and redirect standard input/output streams for applications. It provides developers with tools to manipulate and redirect `stdlog`, `stdout`, and `stderr` programmatically.

## Features

- Redirect standard log, output, and error streams.
- Lightweight and easy to integrate into existing projects.
- Compatible with POSIX systems.

## Usage

Include the library in your project:
```c++
#include <CRedirect.h>
```

Example usage:
```c++
#include <CRedirect.h>

int main() {    
     CoutRedirect coutRedirect{};
     
     cout << "This will be written to cout observer stream." << std::endl();
     return 0;
}
```

## Documentation

Detailed documentation is available in the source code.

## Contributing

Contributions are welcome! Please submit issues or pull requests via GitHub.

## License

This project is licensed under the LGPL v3 License. See the [LICENSE](LICENSE) file for details.