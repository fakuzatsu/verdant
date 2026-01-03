import { Handler } from 'paged.esm.js';

/**
 * @file table-of-content.js
 * @description This script helps you generate a table of contents.
 * Use it as a module with paged.esm.js.
 * 
 * @see https://gitlab.coko.foundation/pagedjs/pagedjs-plugins/table-of-content
 * 
 * Originally developed by Julie Blanc and Julien Taquet.
 */




export class createTocHandler extends Handler {
    constructor(chunker, polisher, caller) {
        super(chunker, polisher, caller);
        this.tocContainer = "#toc"; // ← The element inside you want generate the table of content
        this.tocTitles = ["h2", "h3"]; // ← List of title levels to include in the table of contents
    }

    beforeParsed(content){
      createToc({
          content: content,
          container: config.toc.container, 
          titleElements: this.tocTitles
      });
    }
    
}


function createToc(config) {

  const content = config.content
  const tocElement = config.tocElement
  const titleElements = config.titleElements


  let tocElementDiv = content.querySelector(tocElement)
  if(!tocElementDiv) return console.warn('couldn’t start the toc')
  tocElementDiv.innerHTML = ''
  let tocUl = document.createElement('ul')
  tocUl.id = 'list-toc-generated'
  tocElementDiv.appendChild(tocUl)

  // add class to all title elements
  let tocElementNbr = 0
  for (var i = 0; i < titleElements.length; i++) {
    let titleHierarchy = i + 1
    let titleElement = content.querySelectorAll(titleElements[i])

    titleElement.forEach(function (element) {
      // check if shouldbe shown
      if (
        !(
          element.closest('section').classList.contains('toc-ignore') ||
          element.closest('section').classList.contains('toc')
        )
      ) {
        // add classes to the element
        element.classList.add('title-element')
        element.setAttribute('data-title-level', titleHierarchy)

        // add an id if doesn't exist
        tocElementNbr++

        if (element.id == '') {
          element.id = 'title-element-' + tocElementNbr
        }
        let newIdElement = element.id
      }
    })
  }

  // create toc list
  let tocElements = content.querySelectorAll('.title-element')

  for (var i = 0; i < tocElements.length; i++) {
    let tocElement = tocElements[i]

    let tocNewLi = document.createElement('li')

    // Add class for the hierarcy of toc
    tocNewLi.classList.add('toc-element')
    tocNewLi.classList.add('toc-element-level-' + tocElement.dataset.titleLevel)

    let classes = [
      ...tocElement.className.split(' '),
      ...tocElement.closest('section')?.className.split(' '),
    ]

    classes.forEach((meta) => {
      if (meta == 'title-element' || meta == undefined || meta == '') return
      tocNewLi.classList.add(`toc-${meta}`)
    })

    //get the exisiting class
    // Keep class of title elements
    let classTocElement = tocElement.classList
    for (var n = 0; n < classTocElement.length; n++) {
      if (classTocElement[n] != 'title-element') {
        tocNewLi.classList.add(classTocElement[n])
      }
    }

    // Create the element
    tocNewLi.innerHTML =
      '<a href="#' + tocElement.id + '">' + tocElement.innerHTML + '</a>'
    tocUl.appendChild(tocNewLi)
  }
}
