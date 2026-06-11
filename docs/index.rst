FVMaker Documentation
=====================

FVMaker, or FVM, is a C++ library for finite-volume problems on structured
one-dimensional and two-dimensional grids.

The mesh generation and geometric storage responsibilities belong to
FVGridMaker. FVM consumes FVGridMaker grids and focuses on fields, equations,
assembly, solvers, time control, examples, and book results.

Planning Documents
------------------

The current planning documents are maintained in Markdown while the Sphinx
tree is being established:

* :download:`Functional and non-functional requirements <FVMaker_requisitos.md>`
* :download:`Gate-based development schedule <FVMaker_cronograma_por_gates.md>`
* :download:`Project tree and programming conventions <FVMaker_tree.md>`

Build
-----

Configure documentation with:

.. code-block:: console

   cmake -S . -B build -DBUILD_DOCUMENTATION=ON
   cmake --build build --target docs
