function createToc(config) {
  const content = config.content
  const tocElement = config.tocElement
  const titleElements = config.titleElements

  console.log('starting toc gen')
  let tocElementDiv = content.querySelector(tocElement)
  if(!tocElementDiv) return console.warn('couldn\'t start the toc')
  tocElementDiv.innerHTML = ''

  // // Add the title first
  // let tocTitle = document.createElement('h1')
  // tocTitle.id = 'toc-title'
  // tocTitle.textContent = 'Table of Contents'
  // tocElementDiv.appendChild(tocTitle)

  let tocUl = document.createElement('ul')
  tocUl.id = 'list-toc-generated'
  tocElementDiv.appendChild(tocUl)

  // ... rest of your existing code for processing elements
  let tocElementNbr = 0
  for (var i = 0; i < titleElements.length; i++) {
    let titleHierarchy = i + 1
    let titleElement = content.querySelectorAll(titleElements[i])

    titleElement.forEach(function (element) {
      const parentSection = element.closest('section');
      if (
        !(
          (parentSection && parentSection.classList.contains('toc-ignore')) ||
          (parentSection && parentSection.classList.contains('toc'))
        )
      ) {
        element.classList.add('title-element')
        element.setAttribute('data-title-level', titleHierarchy)
        tocElementNbr++
        if (element.id == '') {
          element.id = 'title-element-' + tocElementNbr
        }
      }
    })
  }

  let tocElements = content.querySelectorAll('.title-element')
  for (var i = 0; i < tocElements.length; i++) {
    let tocElement = tocElements[i]
    let tocNewLi = document.createElement('li')
    tocNewLi.classList.add('toc-element')
    tocNewLi.classList.add('toc-element-level-' + tocElement.dataset.titleLevel)

    let classes = [
      ...tocElement.className.split(' '),
      ...(tocElement.closest('section')?.className.split(' ') || [])
    ]

    classes.forEach((meta) => {
      if (meta == 'title-element' || meta == undefined || meta == '') return
      tocNewLi.classList.add(`toc-${meta}`)
    })

    let classTocElement = tocElement.classList
    for (var n = 0; n < classTocElement.length; n++) {
      if (classTocElement[n] != 'title-element') {
        tocNewLi.classList.add(classTocElement[n])
      }
    }

    tocNewLi.innerHTML = '<a href="#' + tocElement.id + '">' + tocElement.innerHTML + '</a>'
    tocUl.appendChild(tocNewLi)
  }
}