# Wave equation

Although this is not the main focus of this repository, the following code generates an animation that presents solutions to the 2D wave equation.

$$ \frac{\partial^2 u}{\partial t^2} = c^2 (\frac{\partial^2 u}{\partial x^2} + \frac{\partial^2 u}{\partial y^2}) $$

In particular I considered the (simplest) case of circular waves emanating from a single central point. In this case, we can approximate the solution by:

$$ u(r,t) = A \cos (kr - \omega t) $$

where:

- $A$ is the amplitude
- $k$ is the wave number related to wavelength $\lambda$ by $k = \frac{2\pi}{\lambda}$
- $\omega$ is the angular frequency
- $r = \sqrt{x^2 + y^2}$ is the radius or simply the distance from the central point

# Mesh of cells

In [finite element method (FEM)](https://en.wikipedia.org/wiki/Finite_element_method) the space is discretized into a 2D or 3D mesh (grid) of cells. To visualize the system, we want to display each cell on the screen, often assigning a representative colour corresponding to a certain value in the cell.

Here I aim to compare the performance of two mesh implementations: `ShapeGrid`, where each cell is a `sf::RectangleShape` and `ArrayGrid` which uses the efficent `sf:VertexArray`.

# The two implementations

Being an impatient rascal, when I was learning SFML I did not bother reading the tutorials further than the bare minimum I needed to realize my [2D diffusion project](https://github.com/barrgol/diffusion-2d). Once I've read how to render a rectangle using `sf::RectangleShape` I've figured this is all I need. I was surprised to see that the runtime visualization is not very smooth with $200 \times 200$ mesh - I've expected that the slowdown will be visible only with a much bigger grid. Only recently have I learned that one wants to minimize the number of calls to `draw()` to obtain good performance.

The `ShapeGrid` uses the wrong, naive approach with grid represented by a 2D `std::vector` of `sf::RectangleShape`. Rendering such grid is done via a double `for` loop calling `draw()` on each rectangle (cell) separately. 

A better approach is to use `sf::VertexArray` which keeps a list of vertices representing the shapes on the screen and renders them with a single `draw()` call. This implementation is used in `ArrayGrid`, where each cell consists of 6 vertices corresponding to 2 triangles connected via the hypotenuse.

`ShapeGrid` does well on `100 x 100` matrix

https://github.com/user-attachments/assets/cd1273a0-2a83-49ee-af95-4a2c8315c722

But starts to struggle already at `300 x 300` cells

https://github.com/user-attachments/assets/9fe33c43-8861-4a4d-844c-1f969e4d8a66

`300 x 300` matrix is no big challenge to `ArrayGrid`

https://github.com/user-attachments/assets/551ba811-f806-4a37-9ee0-03bd7e794c37

And it only visibly loses performance at `600 x 600`

https://github.com/user-attachments/assets/5fa38c14-6e6a-4725-a463-26a0bf202423

However, it can't be stated for sure whether rendering is the bottleneck anymore. Probably at this point the problem lies in the computation of amplitudes within the mesh. I may try to look into further optimizations in the future.
